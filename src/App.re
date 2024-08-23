/*
 * In OCaml, a module is similar to a TypeScript or JavaScript module.
 * It's a way to group related functions, types, and components. Here, `App`
 * is a module that encapsulates our main app component.
 *
 * The `[@react.component]` attribute is a special annotation in OCaml for
 * React. It's used to define a React component in a way that's more idiomatic
 * to OCaml. This attribute automatically manages the props transformation,
 * providing a seamless integration with React's component model.
 *
 * The `make` function inside the module is equivalent to defining a functional
 * component in React using JavaScript or TypeScript. In ReasonReact, it's
 * common to name the main function of a component `make` instead of the
 * component's name.
 */

type step =
  | Idle /* There is no request going on, and no data to be shown. In this case, we will just show instructions to proceed with the request. */
  | Loading(option(result(Feed.feed, string))) /* A request is currently taking place to fetch the feed. Stores the previously fetched feed. */
  | InvalidUsername /* The entered username is not a valid GitHub ID. */
  | Loaded(result(Feed.feed, string)); /* A request has finished, its result is contained inside the variant value itself. */

type state = {
  username: string,
  step,
  currentPage: int,
};

let mergeFeeds = (oldFeed: Feed.feed, newFeed: Feed.feed): Feed.feed => {
  let combinedEntries = Array.concat([oldFeed.entries, newFeed.entries]);
  {entries: combinedEntries};
};

let renderFeed = (feed: Feed.feed) =>
  <div>
    <h1> {React.string("GitHub Feed")} </h1>
    <ul>
      {switch (feed.entries) {
       | [||] => React.string("This user feed is empty")
       | entries =>
         entries
         |> Array.map((entry: Feed.entry) =>
              <li key={entry.id}>
                <h2> {React.string(entry.title)} </h2>
                {switch (entry.content) {
                 | None => React.null
                 | Some(content) =>
                   <p dangerouslySetInnerHTML={"__html": content} />
                 }}
              </li>
            )
         |> React.array
       }}
    </ul>
  </div>;

module App = {
  [@react.component]
  let make = () => {
    let (state, setState) =
      React.useState(() =>
        {username: "jchavarri", step: Idle, currentPage: 1}
      );
    let sentinelRef = React.useRef(Js.Nullable.null);

    let fetchFeed = (~username, ~page) => {
      setState(state =>
        {
          ...state,
          step:
            switch (state.step) {
            | Loaded(r)
            | Loading(Some(r)) => Loading(Some(r))
            | Loading(None)
            | Idle
            | InvalidUsername => Loading(None)
            },
        }
      );
      module P = Js.Promise;
      Fetch.fetch(
        "https://gh-feed.vercel.app/api?user="
        ++ Username.toString(username)
        ++ "&page="
        ++ string_of_int(page),
      )
      |> P.then_(response => {
           let status = Fetch.Response.status(response);
           if (status === 200) {
             /* If status is OK, proceed to parse the response */
             response
             |> Fetch.Response.text
             |> P.then_(text =>
                  {
                    let data =
                      try(Ok(text |> Json.parseOrRaise |> Feed.feed_of_json)) {
                      | Json.Decode.DecodeError(msg) =>
                        Js.Console.error(msg);
                        Error("Failed to decode: " ++ msg);
                      };

                    switch (data) {
                    | Error(_) =>
                      setState(state => {...state, step: Loaded(data)})
                    | Ok(data) =>
                      setState(state => {
                        let updatedFeed =
                          switch (state.step) {
                          | Loaded(Ok(feed))
                          | Loading(Some(Ok(feed))) =>
                            mergeFeeds(feed, data)
                          | _ => data
                          };
                        {
                          ...state,
                          step: Loaded(Ok(updatedFeed)),
                          currentPage: page + 1,
                        };
                      })
                    };
                  }
                  |> P.resolve
                );
           } else {
             /* Handle non-200 status */
             setState(state =>
               {
                 ...state,
                 step:
                   Loaded(
                     Error(
                       "Error: Received status " ++ string_of_int(status),
                     ),
                   ),
               }
             )
             |> P.resolve;
           };
         })
      |> ignore;
    };

    React.useEffect1(
      () => {
        switch (Username.make(state.username)) {
        | Error () =>
          Js.Exn.raiseError("The value of state.username is invalid")
        | Ok(username) =>
          switch (Js.Nullable.toOption(sentinelRef.current)) {
          | None =>
            ();
            None;
          | Some(elem) =>
            let shouldFetch =
              switch (state.step) {
              | Loading(_) => false
              | Loaded(Ok({entries: [||]})) => false
              | Idle
              | InvalidUsername
              | Loaded(_) => true
              };
            let observer =
              IntersectionObserver.make(entries => {
                let entry = entries[0];
                if (entry.isIntersecting && shouldFetch) {
                  fetchFeed(~username, ~page=state.currentPage);
                };
              });
            IntersectionObserver.observe(observer, elem);
            Some(() => IntersectionObserver.disconnect(observer));
          }
        }
      },
      [|state|],
    );

    <div>
      <UsernameInput
        username={state.username}
        onChange={value => {
          setState(_ => {...state, username: value, step: Idle})
        }}
        onEnterKeyDown={() =>
          switch (Username.make(state.username)) {
          | Ok(username) => fetchFeed(~username, ~page=1)
          | Error () => setState(state => {...state, step: InvalidUsername})
          }
        }
      />
      {switch (state.step) {
       | InvalidUsername => <div> {React.string("Invalid username")} </div>
       | Idle =>
         <div>
           {React.string(
              "Press the \"Enter\" key to confirm the username selection.",
            )}
         </div>
       | Loading(None | Some(Error(_))) =>
         <div> {React.string("Loading...")} </div>
       | Loading(Some(Ok(feed))) =>
         <> {renderFeed(feed)} <div> {React.string("Loading...")} </div> </>
       | Loaded(Error(msg)) => <div> {React.string(msg)} </div>
       | Loaded(Ok(feed)) => renderFeed(feed)
       }}
      <div ref={ReactDOM.Ref.domRef(sentinelRef)} />
    </div>;
  };
};

ReactDOM.querySelector("#root")
->(
    fun
    | Some(root_elem) => {
        let root = ReactDOM.Client.createRoot(root_elem);
        ReactDOM.Client.render(root, <App />);
      }
    | None =>
      Js.Console.error(
        "Failed to start React: couldn't find the #root element",
      )
  );

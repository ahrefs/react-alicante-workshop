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

type loadingStatus =
  | Loading
  | Loaded(result(Feed.feed, string));

module App = {
  [@react.component]
  let make = () => {
    let (data, setData) = React.useState(() => Loading);
    let (username, setUsername) = React.useState(() => "jchavarri");
    let (debounceTimeout, setDebounceTimeout) = React.useState(() => None);

    let fetchFeed = user => {
      module P = Js.Promise;
      Fetch.fetch("https://gh-feed.vercel.app/api?user=" ++ user ++ "&page=1")
      |> P.then_(Fetch.Response.text)
      |> P.then_(text =>
           {
             let data =
               try(Ok(text |> Json.parseOrRaise |> Feed.feed_of_json)) {
               | Json.Decode.DecodeError(msg) =>
                 Js.Console.error(msg);
                 Error("Failed to decode: " ++ msg);
               };
             setData(_ => Loaded(data));
           }
           |> P.resolve
         )
      |> ignore;
    };

    React.useEffect1(
      () => {
        /* Clear the previous timeout if it exists */
        switch (debounceTimeout) {
        | None => ()
        | Some(timeout) => Js.Global.clearTimeout(timeout)
        };

        /* Set a new timeout */
        let newTimeout =
          Js.Global.setTimeout(~f=() => fetchFeed(username), 500);

        /* Store the new timeout ID */
        setDebounceTimeout(_ => Some(newTimeout));
        None;
      },
      [|username|],
    );

    <div>
      <div>
        <label htmlFor="username-input"> {React.string("Username:")} </label>
        <input
          id="username-input"
          value=username
          onChange={event => {
            setUsername(event->React.Event.Form.target##value);
            setData(_ => Loading);
          }}
          placeholder="Enter GitHub username"
        />
      </div>
      {switch (data) {
       | Loading => <div> {React.string("Loading...")} </div>
       | Loaded(Error(msg)) => <div> {React.string(msg)} </div>
       | Loaded(Ok(feed)) =>
         <div>
           <h1> {React.string("GitHub Feed")} </h1>
           <ul>
             {feed.entries
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
              |> React.array}
           </ul>
         </div>
       }}
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

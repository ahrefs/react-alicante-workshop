## Step 11: Dynamic data loading when the scroll reaches the bottom
## Step 12: Styling with styled-ppx
## Step 13: Async / await
## Step 14: Using variants and `useReducer` to model state machines
## Step 15: Testing our user validation function
## Step X: Questions / custom requests


## TODO
- Remove inner `App` component (make sure to update README)
- Update solutions to remove local opens in Js.Promise on 7.1


## AFTER
- If you feel adventurous, you can modify the [server side
  code](https://github.com/anmonteiro/gh-feed-reader/) to add username
  validation so that it doesn't return 500 errors. Now that you know some
  Melange, OCaml should be an easy step away!

INPUT STYLES

    .input-container {
      margin-bottom: 20px;
      display: flex;
      align-items: center;
    }

    .input-container label {
      margin-right: 10px;
      font-weight: bold;
    }

    .input-container input {
      padding: 8px;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 16px;
      width: 250px;
    }

    .input-container input:focus {
      outline: none;
      border-color: #007bff;
      box-shadow: 0 0 5px rgba(0, 123, 255, 0.5);
    }



    type componentState =
  | Typing
  | InvalidUsername
  | Loading(Username.t)
  | Loaded(result(Feed.feed, string));

type state = {
  usernameInput: string,
  state: componentState,
};

type action = 
| EnterKeyPressed
| UsernameInputChanged(string)
| ComponentLoaded;

let reducer = (action, state) => {
  switch(action) {
    | EnterKeyPressed => switch (Username.make(state.usernameInput)) {
      | 
    }
| UsernameInputChanged(string)
| ComponentLoaded;

  }
}

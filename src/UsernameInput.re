[@mel.module "./UsernameInput.module.css"]
external container: string = "container";

[@react.component]
let make = (~username, ~onChange, ~onEnterKeyDown) =>
  <div className=container>
    <label htmlFor="username-input"> {React.string("Username:")} </label>
    <input
      id="username-input"
      value=username
      onChange={event => onChange(event->React.Event.Form.target##value)}
      onKeyDown={event => {
        let enterKey = 13;
        if (React.Event.Keyboard.keyCode(event) == enterKey) {
          onEnterKeyDown();
        };
      }}
      placeholder="Enter GitHub username"
    />
  </div>;

/* Externals allow to interact with existing JavaScript code or modules. In this
   case, we want to "import" an image:
   https://melange.re/v4.0.0/communicate-with-javascript.html#using-functions-from-other-javascript-modules
   The code below will generate `import CamelFunJpg from "./img/camel-fun.jpg";`
   so that Esbuild can process the asset later on.
   */
[@mel.module "./img/camel-fun.jpg"] external camelFun: string = "default";

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
module App = {
  [@react.component]
  let make = () => {
    <>
      <div />
      <div>
        <h1>
          {/*
            * `React.string` is used in ReasonReact to convert plain strings into
            * React text elements. This is necessary because in OCaml, you
            * can't directly render strings in JSX. It's similar to how text
            * content is treated in JSX/TSX in the JavaScript/TypeScript world,
            * but with explicit conversion for better type safety and clarity in
            * the OCaml ecosystem.
            */
           React.string(
             "Hello Alicante!",
           )}
        </h1>
        <h2>
          {React.string("Write maintainable React applications using OCaml")}
        </h2>
        <img src=camelFun />
      </div>
    </>;
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

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
      <div className="h-[15%]" />
      <div
        className="flex flex-col items-center justify-center rounded-3xl bg-gradient-to-b from-[#24273a] to-[#181926] p-6 shadow outline outline-2 outline-[#f5bde6]">
        <h1
          className="mb-2 pb-1 bg-gradient-to-r from-[#f5bde6] to-[#c6a0f6] bg-clip-text text-7xl font-black text-transparent">
          /*
           * `React.string` is used in ReasonReact to convert plain strings into
           * React text elements. This is necessary because in OCaml, you
           * can't directly render strings in JSX. It's similar to how text
           * content is treated in JSX/TSX in the JavaScript/TypeScript world,
           * but with explicit conversion for better type safety and clarity in
           * the OCaml ecosystem.
           */
           {React.string("Write maintainable React applications using OCaml")} </h1>
        <h2
          className="mb-3 border-b-2 bg-gradient-to-r from-[#ee99a0] to-[#f5a97f] bg-clip-text text-4xl  font-black text-transparent pb-1">
          {React.string("Hello Alicante!")}
        </h2>
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

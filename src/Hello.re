[@react.component]
let make = () =>
  <>
    <h1>
       {React.string("Hello Alicante!")} </h1>
      /*
       * `React.string` is used in ReasonReact to convert plain strings into
       * React text elements. This is necessary because in OCaml, you
       * can't directly render strings in JSX. It's similar to how text
       * content is treated in JSX/TSX in the JavaScript/TypeScript world,
       * but with explicit conversion for better type safety and clarity in
       * the OCaml ecosystem.
       */
    <h2>
      {React.string("Write maintainable React applications using OCaml")}
    </h2>
  </>;

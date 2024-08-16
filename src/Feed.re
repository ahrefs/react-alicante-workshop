open Ppx_deriving_json_runtime.Primitives;

[@deriving json]
type link = {
  href: string,
  title: string,
};

[@deriving json]
type entry = {
  content: option(string),
  id: string,
  links: array(link),
  title: string,
  updated: float,
};

[@deriving json]
type feed = {entries: array(entry)};

let data = {| {
  "entries": [
    {
      "content": "<div>Hello</div>",
      "id": "abcd1234",
      "links": [
        {
          "title": "",
          "href": "https://github.com/melange-community/melange-json"
        }
      ],
      "title": "jchavarri starred melange-community/melange-json",
      "updated": 1723639727000.0
    }
  ]
} |};

let demoFeed =
  try(Some(data |> Json.parseOrRaise |> feed_of_json)) {
  | Json.Decode.DecodeError(msg) =>
    Js.log(msg);
    None;
  };

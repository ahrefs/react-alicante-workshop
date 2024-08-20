type link = {
  href: string,
  title: string,
};

type entry = {
  content: option(string),
  id: string,
  links: array(link),
  title: string,
  updated: float,
};

type feed = {entries: array(entry)};

module Decode = {
  let link = json =>
    Json.Decode.{
      href: json |> field("href", string),
      title: json |> field("title", string),
    };
  let entry = json =>
    Json.Decode.{
      content: json |> optional(field("content", string)),
      id: json |> field("id", string),
      links: json |> field("links", array(link)),
      title: json |> field("title", string),
      updated: json |> field("updated", float),
    };

  let feed = json =>
    Json.Decode.{entries: json |> field("entries", array(entry))};
};

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
  try(Some(data |> Json.parseOrRaise |> Decode.feed)) {
  | Json.Decode.DecodeError(msg) =>
    Js.log(msg);
    None;
  };

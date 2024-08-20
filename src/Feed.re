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


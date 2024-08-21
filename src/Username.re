type t = string;

let make = username => {
  let re = [%re "/^[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,37}[a-zA-Z0-9])?$/"];
  Js.Re.test(~str=username, re) ? Ok(username) : Error();
};

let toString = username => username;

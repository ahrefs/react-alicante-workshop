// IntersectionObserver bindings
type entry = {
  isIntersecting: bool,
  target: Dom.element,
};

type observer;
[@mel.send] external observe: (observer, Dom.element) => unit = "observe";
[@mel.send] external unobserve: (observer, Dom.element) => unit = "unobserve";
[@mel.send] external disconnect: observer => unit = "disconnect";

[@mel.new] external make: (array(entry) => unit) => observer = "IntersectionObserver";


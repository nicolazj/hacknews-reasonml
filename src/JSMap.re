type map('a, 'b) =
  | JSMap('a, 'b);

[@bs.new] external create_map : unit => map('a, 'b) = "Map";

[@bs.send] external set : (map('a, 'b), 'a, 'b) => unit = "set";

[@bs.send] [@bs.return nullable]
external get : (map('a, 'b), 'a) => option('b) = "get";

let create = pairs => {
  let empty_map = create_map();
  Js.Array.reduce(
    (m, (a, b)) => {
      set(m, a, b);
      m;
    },
    empty_map,
    pairs
  );
};
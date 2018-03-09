open Util;

[%bs.raw {|require('./app.css')|}];

type action =
  | Home
  | Comment(int);

type state = {page: action};

let component = ReasonReact.reducerComponent("App");

let arr = [|"1", "2"|];

let mapping = path =>
  switch path {
  | [""] => Home
  | ["comment", commentId] => Comment(int_of_string(commentId))
  | _ => Home
  };

let make = _children => {
  ...component,
  initialState: () => {
    page: mapping(ReasonReact.Router.dangerouslyGetInitialUrl().path)
  },
  reducer: (action, _) =>
    switch action {
    | Home => ReasonReact.Update({page: Home})
    | Comment(id) => ReasonReact.Update({page: Comment(id)})
    },
  render: ({state}) =>
    <div className="App">
      (
        switch state.page {
        | Home => <Hot />
        | Comment(id) => <span> <Comment id /> </span>
        }
      )
    </div>,
  subscriptions: self => [
    Sub(
      () => ReasonReact.Router.watchUrl(url => self.send(mapping(url.path))),
      ReasonReact.Router.unwatchUrl
    )
  ]
};
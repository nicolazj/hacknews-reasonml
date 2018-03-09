type state = {
  stories: Story.stories,
  page: int,
  loading: bool
};

type actions =
  | Loaded((int, Story.stories));

let component = ReasonReact.reducerComponent("List");

let make = _children => {
  ...component,
  initialState: () => {stories: [||], page: 0, loading: false},
  reducer: (action, _state) =>
    switch action {
    | Loaded((page, stories)) =>
      ReasonReact.Update({stories, page, loading: false})
    },
  didMount: self => {
    Story.fetchTopStories(0, payload => self.send(Loaded(payload)));
    ReasonReact.NoUpdate;
  },
  render: self =>
    <ul>
      (
        self.state.stories
        |> Array.mapi((index, story) =>
             <li key=(string_of_int(index))> <Item story /> </li>
           )
        |> ReasonReact.arrayToElement
      )
    </ul>
};
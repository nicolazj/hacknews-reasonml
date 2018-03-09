open Util;

let component = ReasonReact.statelessComponent("Item");

let make = (~story: Story.story, _) => {
  let onClick = (id, _) => ReasonReact.Router.push({j| /comment/$id |j});
  {
    ...component,
    render: self =>
      <div>
        <a onClick=((_) => self.handle(onClick, story.id,))>
          (s2e(story.title))
        </a>
      </div>
  };
};
open Util;

type action =
  | Loaded(Story.storyWithComments);

type state = {story: option(Story.storyWithComments)};

let component = ReasonReact.reducerComponent("Comment");

let make = (~id, _) => {
  let rec renderComments = (story: Story.storyWithComments, kids) => {
    let dom =
      switch kids {
      | Some(comments) =>
        comments
        |> Array.map(commentId => {
             let mayBeComment = JSMap.get(story.comments, commentId);
             switch mayBeComment {
             | Some(comment) =>
               switch comment {
               | Story.CommentPresent(comment) =>
                 <li key=(string_of_int(comment.id))>
                   <div
                     dangerouslySetInnerHTML=(dangerousHtml(comment.text))
                   />
                   (renderComments(story, comment.kids))
                 </li>
               | Story.CommentDeleted(_) => <li> (s2e("delete")) </li>
               }
             | None => ReasonReact.nullElement
             };
           })
        |> ReasonReact.arrayToElement
      | None => ReasonReact.nullElement
      };
    <ol
      style=(ReactDOMRe.Style.make(~backgroundColor="rgba(255,0,0,0.1)", ()))>
      dom
    </ol>;
  };
  {
    ...component,
    reducer: (action, _state) =>
      switch action {
      | Loaded(story) => ReasonReact.Update({story: Some(story)})
      },
    initialState: () => {story: None},
    didMount: self => {
      Story.fetchStoryWithComments(id, payload => self.send(Loaded(payload)));
      ReasonReact.NoUpdate;
    },
    render: ({state}) =>
      switch state.story {
      | Some(story) =>
        <div> (s2e(story.title)) (renderComments(story, story.kids)) </div>
      | _ => <span />
      }
  };
};
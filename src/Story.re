let apiBaseUrl = "https://serverless-api.hackernewsmobile.com";

let topStoriesUrl = page => {j|$apiBaseUrl/topstories-25-$page.json|j};

let storyUrl = id => {j|$apiBaseUrl/stories/$id.json|j};

type story = {
  by: string,
  descendants: int,
  id: int,
  score: int,
  time: int,
  title: string,
  url: option(string)
};

type stories = array(story);

type comment_present = {
  id: int,
  kids: option(array(int)),
  parent: int,
  text: string,
  time: int
};

type comment_deleted = {
  id:int,
}; 

type comment = 
  | CommentPresent(comment_present)
  | CommentDeleted(comment_deleted); 

type comments_map = JSMap.map(int,comment); 
    

type storyWithComments = {
  comments: comments_map,
  descendants: int,
  descendentIds: array(int),
  kids: option(array(int)),
  score: int,
  time: int,
  title: string,
  url: option(string)
};
  

module Decode = {
  let idsArray = json : array(int) => Json.Decode.(json |> array(int));
  let story = json : story =>
    Json.Decode.{
      by: json |> field("by", string),
      descendants: json |> field("descendants", int),
      id: json |> field("id", int),
      score: json |> field("score", int),
      time: json |> field("time", int),
      title: json |> field("title", string),
      url: json |> optional(field("url", string))
    };
  let stories = json : array(story) => Json.Decode.(json |> array(story));
  let comment = json : comment =>{
    let isDeleted = Json.Decode.( json |>  optional(field("deleted", bool)));
    let deleted = switch(isDeleted){
    | Some(v) => v == true
    | None => false
    }; 
    if(deleted){
      CommentDeleted(Json.Decode.{
        id: json |> field("id", int),
      })
    }else{
      CommentPresent(Json.Decode.{
        id: json |> field("id", int),
        kids: json |> optional(field("kids", json => json |> array(int))),
        parent: json |> field("parent", int),
        text: json |> field("text", string),
        time: json |> field("time", int)
      })
    }
  };
  let comments = json : array(comment) => Json.Decode.(json |> array(comment));

  let getId = comment =>{
    switch(comment){
    | CommentPresent(comment) => comment.id
    | CommentDeleted(comment) => comment.id
    }
  };
  
  let comments_map = json: comments_map =>{
    let commentsArray = json |> comments;
     
    JSMap.create(commentsArray |> Array.map(comment=>(getId(comment),comment)))
  };
 
  
  let storyWithComments = json : storyWithComments =>
    Json.Decode.{
      comments: json |> field("comments", comments_map),
      descendants: json |> field("descendants", int),
      descendentIds: json |> field("descendentIds", json => json |> array(int)),
      kids: json |> optional(field("kids", json => json |> array(int))),
      score: json |> field("score", int),
      time: json |> field("time", int),
      title: json |> field("title", string),
      url: json |> optional(field("url", string))
    };
};

let fetchTopStories = (page, callback) =>
  Js.Promise.(
    Fetch.fetch(topStoriesUrl(page))
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> Decode.stories
         |> (
           stories => {
             callback((page, stories));
             resolve();
           }
         )
       )
    |> ignore /* TODO: error handling */
  );

let fetchStoryWithComments = (id, callback) =>
  Js.Promise.(
    Fetch.fetch(storyUrl(id))
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> Decode.storyWithComments
         |> (
           stories => {
             callback(stories);
             resolve();
           }
         )
       )
    |> ignore /* TODO: error handling */
  );
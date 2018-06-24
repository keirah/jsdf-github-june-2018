type state = {repoData: option(RepoData.repo)};

type action =
  | PlaceholderAction;

let component = ReasonReact.reducerComponent("App");

let dummyRepo: RepoData.repo = {
  full_name: "jsdf/reason-react-hacker-news",
  stargazers_count: 27,
  html_url: "https://github.com/jsdf/reason-react-hacker-news",
};

let make = _children => {
  ...component,
  initialState: () => {repoData: Some(dummyRepo)},
  reducer: (action, _state) =>
    switch (action) {
    | PlaceholderAction => ReasonReact.NoUpdate
    },
  render: self => {
    let repoItem =
      switch (self.state.repoData) {
      | Some(repo) => <RepoItem repo />
      | None => ReasonReact.string("Loading")
      };
    <div> <h1> (ReasonReact.string("Reason Projects")) </h1> repoItem </div>;
  },
};
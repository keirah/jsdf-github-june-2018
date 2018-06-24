type state = {repoData: option(RepoData.repo)};

type action =
  | Loaded(RepoData.repo);

let component = ReasonReact.reducerComponent("App");

let dummyRepo: RepoData.repo = {
  full_name: "jsdf/reason-react-hacker-news",
  stargazers_count: 27,
  html_url: "https://github.com/jsdf/reason-react-hacker-news",
};

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  reducer: (action, _state) =>
    switch (action) {
    | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
    },
  render: self => {
    let loadedReposButton =
      <button onClick=(_event => self.send(Loaded(dummyRepo)))>
        (ReasonReact.string("Load Repos"))
      </button>;
    let repoItem =
      switch (self.state.repoData) {
      | Some(repo) => <RepoItem repo />
      | None => loadedReposButton
      };
    <div> <h1> (ReasonReact.string("Reason Projects")) </h1> repoItem </div>;
  },
};
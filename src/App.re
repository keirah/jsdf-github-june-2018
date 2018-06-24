type state = {repoData: option(array(RepoData.repo))};

type action =
  | Loaded(array(RepoData.repo));

let component = ReasonReact.reducerComponent("App");

let dummyRepo: array(RepoData.repo) = [|
  {
    full_name: "jsdf/reason-react-hacker-news",
    stargazers_count: 27,
    html_url: "https://github.com/jsdf/reason-react-hacker-news",
  },
  {
    full_name: "reasonml/reason-tools",
    stargazers_count: 93,
    html_url: "https://github.com/reasonml/reason-tools",
  },
|];

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
    let repoItems =
      switch (self.state.repoData) {
      | Some(repos) =>
        ReasonReact.array(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
            repos,
          ),
        )
      | None => loadedReposButton
      };
    <div> <h1> (ReasonReact.string("Reason Projects")) </h1> repoItems </div>;
  },
};
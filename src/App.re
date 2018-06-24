let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => {
    let dummyRepo: RepoData.repo = {
      full_name: "jsdf/reason-react-hacker-news",
      stargazers_count: 27,
      html_url: "https://github.com/jsdf/reason-react-hacker-news",
    };
    <div>
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      <RepoItem repo=dummyRepo />
    </div>;
  },
};
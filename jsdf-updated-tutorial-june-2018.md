The following post updates James Friend's [excellent ReasonReact tutorial](https://jamesfriend.com.au/a-first-reason-react-app-for-js-developers) for the most recent ReasonReact and BuckleScript as at June 24, 2018 (`bs-platform@3.1.5` and `reason-react@0.4.2`).

# James Friend

## Web Platform Adventures & PC Archeology

# A First Reason React app for Javascript developers

Submitted by James on November 14, 2017

This post has been updated to use the new Reason 4 syntax, and to account for API changes in Reason React. A Traditional Chinese translation of an older version of this article is available [here](http://987.tw/a-first-reason-react-app-for-javascript-developers/).

[Reason](https://reasonml.github.io/) is a new statically-typed functional programming language from Facebook which can be compiled to Javascript. [Reason React](https://reasonml.github.io/reason-react/) is a wrapper for [React](https://reactjs.org/) which makes it easy to use from Reason.

We're going to build a small single page web app to put Reason React through its paces. The app will display a list of top Reason-related Github repos. It's a small enough task that we can complete it in a few hours, but also has enough complexity that we can kick the tires of this new language. This tutorial expects no existing knowledge of Reason, though a basic familiarity with static types would be helpful.

### Before we get started

Make sure you have your editor set up for Reason. You're not getting the full benefit of a statically typed language if you haven't got type information, inline errors and autocomplete in your editor.

Your editor plug-ins will rely on the [**Reason-CLI tools**](https://reasonml.github.io/docs/en/global-installation.html), so follow the instructions [here](https://reasonml.github.io/docs/en/global-installation.html) to get started, before getting setup with the [recommended editor plugins](https://reasonml.github.io/docs/en/editor-plugins.html)

## A new project

We're going to use the [preferred option](https://reasonml.github.io/reason-react/docs/en/installation.html) for creating starter project, which will create a starting point for our app, which is going to be called `reason-repo-list`:

> Please note that the official version of this [tutorial](https://jamesfriend.com.au/a-first-reason-react-app-for-js-developers) used [create-react-app](https://github.com/facebook/create-react-app) with the Reason-specific addon [reason-scripts](https://github.com/reasonml-community/reason-scripts). People have occasionally reported issues with this however, so let's go with the ReasonReact recommended option (even though it means opening up a couple more terminal windows).

To start, first we install BuckleScript via `bs-platform` (the reason-to-js compiler) and then create our starter project:

```
npm install -g bs-platform
bsb -init reason-repo-list -theme react
cd reason-repo-list && npm install && npm start
```

And then in another terminal tab:

```
npm run webpack
```

And in another tab:

```
open src/index.html
```

I'll go into more detail about what's going on under the hood later, right now we just want to get something on the screen.

Right now, if you've opened `src/index.html` you should see this:

![Screenshot of starter project](src/images/reason-react-starter-june-2018.png)

The starter project provides you with two components, `Component1.re` and `Component2.re` as examples. You can keep them in your project for reference, but we're not going to use them here. Instead, please open up `src/index.html` in your editor, and replace the entire contents with this:

```
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <title>ReasonReact Tutorial</title>
</head>

<body>
  Adapted from
  <a href="https://jamesfriend.com.au/a-first-reason-react-app-for-js-developers">this tutorial</a> by James Friend.
  <div id="root"></div>
  <script src="../build/Index.js"></script>
</body>

</html>
```

Next, In your editor, open the project folder and open up src/Index.re. Some of the stuff in there might look a bit scary. Don't worry! Just delete and replace the whole contents of the file with this:

```
ReactDOMRe.renderToElementWithId(<App />, "root");
```

If you've built many React apps this should look pretty familiar. The above Reason code is doing roughly the same thing as this Javascript equivalent:

```
ReactDOM.render(<App />, document.getElementById('root'));
```

### We've found a bug for you!

At this point, you should have the following error message in your console:

```

  /Users/keira/reasonml/june-2018/reason-repo-list/src/Index.re 1:34-40

  1 │ ReactDOMRe.renderToElementWithId(<App />, "root");

  The module or file App can't be found.
  - If it's a third-party dependency:
    - Did you list it in bsconfig.json?
    - Did you run `bsb` instead of `bsb -make-world`
      (latter builds third-parties)?
  - Did you include the file's directory in bsconfig.json?
```

Let's create a new file to address this in the - App.re.

## JSX in Reason

Let's move over to src/App.re. This is the top level component of our app. Set up App.re so that it contains the following code:

```
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <div> (ReasonReact.string("Reason Projects")) </div>,
};
```

This page is being rendered using React, from a component written in Reason.
Hit save and jump back to your browser window showing `src/index.html`, and refresh. You should see a page which just says 'Reason Projects'.

Jump back to your editor and we'll walk through the code. It looks somewhat like the React JSX you're used to, with a few differences.

In Reason React, some things are a bit more explicit than normal Javascript React. Reason's JSX doesn't allow you to display text by simply putting it directly between JSX tags. Instead we use a function called ReasonReact.stringToElement, and we call it with the string of text we want to display: "Reason Projects". In Reason strings are always double quoted.

You can think of the above code as being more or less equivalent to this JS React code:

```
class App extends React.Component {
  render() {
    return (
      <div className="App">
        <div className="App-header">
          <h1>{'Reason Projects'}</h1>
        </div>
      </div>
    );
  }
}
```

## Debugging syntax errors

While you're getting started with Reason, you might make an error in your code. If you do, an error message will show in the browser (if you're using Reason-Scripts) or the terminal window.

If the error you've made is a syntax error, the message in the browser can be a bit confusing. The file, line and column location of the syntax error actually appears in the middle of the error message, on the line before the word `Error`.

![Screenshot of the app with a syntax error](src/images/reason-react-starter-june-2018-syntax-error.png)

Similarly, it can be a bit difficult to spot where exactly you've made the error when looking at your code in the editor.

If the first error message in the file within your editor you see is `<UNKNOWN SYNTAX ERROR>`, you're dealing with a syntax error, so you should look at the message in the browser (Reason Scripts) or terminal window where you're running `npm start` (`bsb -init` method).

## A record type

Next, our list of repos. First we'll build out the UI components with fake data, and then replace it with data from this API request: [https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories](https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories)

We'll define a record type to represent each repo item from the JSON. A record is like a JS object, except that the list of properties that it has and what their types are is fixed. This is how we might define a record type for Github API data about a Github repo:

```
type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
};
```

Create a new file called `RepoData.re` and add the above code into it.

## Files are modules

We've defined our type at the top level of the file. In Reason, every file is a module, and all the things defined at the top level of the file using the keywords let, type, and module are exposed to be used from other files (that is, other modules). In this case, other modules can reference our repo type as RepoData.repo. Unlike in Javascript, no imports are required to reference things from other modules.

Let's use our type in App.re. The repos page is just a list of repos, with each item in the list consisting of the name of the repo (linking to the repo on Github), and the number of stars the repo has. We'll define some dummy data and sketch out a new component called RepoItem to represent an item in the list of repos:

```
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => {
	 /* our dummy data */
    let dummyRepo: RepoData.repo = {
      full_name: "jsdf/reason-react-hacker-news",
      stargazers_count: 27,
      html_url: "https://github.com/jsdf/reason-react-hacker-news",
    };
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      <RepoItem repo=dummyRepo />
    </div>;
  },
};
```

In the statement beginning `let dummyRepo: RepoData.repo =`, dummyRepo is the name of the constant we're defining and `RepoData.repo` is the type we're annotating it with. Reason can infer the types of most things we declare, but here it's useful to include the annotation so that the typechecker can let us know if we've made a mistake in our test data.

### Return values in Reason

Note that the body of the render function is now wrapped in `{}` braces, because it contains multiple statements. In Javascript, if we used braces around the body of an `=>` arrow function we'd need to add a return statement to return a value. However in Reason, value resulting from the last statement in the function automatically becomes the return value. If you don't want to return anything from a function, you can make the last statement `()` (which is called 'unit').

## Defining components in Reason React

You might now see an error saying The module or file RepoItem can't be found. That's because we added <RepoItem repo=dummyRepo /> in the render function of the App component, but we haven't created that module yet. Add a new file called `RepoItem.re` containing:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) => {
  ...component,
  render: _self => <div> (ReasonReact.string(repo.full_name)) </div>,
};
```

What's going on here? Let's dissect what's happening in this file.

Each Reason React component is a Reason module which defines a function called make, which defines props and children arguments. The props are specified as Labelled Arguments.

```
let make = (~someProp, ~anotherProp, _children) => /* some stuff here */;
```

This make function returns a record. The first thing in this record is typically ...component, where component is the return value of `ReasonReact.reducerComponent` or `ReasonReact.statelessComponent` (for components which do and don't use state, respectively). If this seems a bit weird, just think of it as inheriting from the React component class, like the equivalent of doing class Foo extends React.Component { in JS React.

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~someProp, ~anotherProp, _children) =>
  {
    ...component,
    /* render and lifecycle methods go here */
  };
```

The rest of the record is where you can add the render function and the lifecycle methods you're used to from React.

So back to `RepoItem.re`:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) => {
  ...component,
  render: _self => <div> (ReasonReact.string(repo.full_name)) </div>,
};
```

What we have here is a stateless component which takes one prop called repo, (annotated with the type repo from the RepoData module), and renders a div.

In JS React can use this.props to access the component's props inside the render method. In Reason React we instead receive the props as labeled arguments to the make function, and we can use them inside our render function directly (as we are doing to access the `full_name` field of the repo prop above).

The `make` function also get passed a children argument, but we aren't making use of children in this component so we put an \_ underscore at the start of the \_children argument name. That just lets Reason know we're not actually using that argument. Despite the fact we're not using this argument, it does still need to be included in the function arguments or you'll get an error.

Next we'll flesh out the render function to present the fields of the repo record:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) => {
  ...component,
  render: _self =>
    <div className="RepoItem">
      <a href=repo.html_url>
        <h2> (ReasonReact.string(repo.full_name)) </h2>
      </a>
      (ReasonReact.string(string_of_int(repo.stargazers_count) ++ " stars"))
    </div>,
};
```

Note that we have to convert the `int` value of `repo.stargazers_count` to a string using the `string_of_int` function. We then use the `++` string concatenation operator to combine it with the string `" stars"`.

Now is a good time to save and take a look at our progress in the browser.

## A stateful React component

Our app is going to load some data and then render it, which means we need a place to put the data after it's loaded. React component state seems like an obvious choice. So we'll make our App component stateful. We do that by changing our ReasonReact.statelessComponent to a ReasonReact.reducerComponent.

In app.re:

```
type state = {repoData: RepoData.repo};

type action =
  | PlaceholderAction;

let component = ReasonReact.reducerComponent("App");

let dummyRepo: RepoData.repo = {
  stargazers_count: 27,
  full_name: "jsdf/reason-react-hacker-news",
  html_url: "https://github.com/jsdf/reason-react-hacker-news"
};

let make = _children => {
  ...component,
  initialState: () => {repoData: dummyRepo},
  reducer: (action, _state) =>
    switch (action) {
    | PlaceholderAction => ReasonReact.NoUpdate
    },
  render: self =>
    <div className="App">
      <h1>{ReasonReact.string("Reason Projects")}</h1>
      <RepoItem repo=self.state.repoData />
    </div>,
};
```

We've changed some key things:

- we've defined a type for the state of our component, called state,
- ReasonReact.statelessComponent has become ReasonReact.reducerComponent,
- we've added an initialState function to the component, and
- we've changed render to take `self` as it's argument (removing the leading \_ underscore), which is now being used to pass self.state.repoData as a prop to RepoItem.

## Common errors:

Note that the state type, and action type must be defined before the call to ReasonReact.reducerComponent or you'll get an error saying something like

> "Is this a ReasonReact reducerComponent or component with retained props?
> If so, is the type for state, retained props or action declared _after_
> the component declaration?
> Moving these types above the component declaration should resolve this!

## Option and pattern matching

Currently we have our repoData dummy data already available when we define the initial state of the component, but once we are loading it from the server it will initially be `null`. However, in Reason you can't just have the value of a record field be `null`, as you can in Javascript. Instead, things which might not be present need to be 'wrapped' in another type called option. We can change our state type to represent this like so:

```
type state = {repoData: option(RepoData.repo)};
```

and in our initialState function we wrap our repo record in `Some()`:

```
  initialState: () => {
    repoData: Some(dummyRepo),
  },
```

In Reason option is a type which is made up of 'Variants'. That basically means that a value of this type can be one of several possible variations which have been explicitly defined. In the case of option, the variants are `Some` and `None`. Some is used when a value is present (and contains the value itself), whereas None represents the absence of a value (like null in Javascript). Here we've 'wrapped' dummyRepo in the `Some` variant, because we have a value present.

So why use this wrapper, instead of just allowing our repoData field to contain either a value or null? The reason is to force us to handle both possible cases when actually using the value. This is good because it means we can't accidentally forget to deal with the `'null'` case.

This means we also need to change the place where the `repoData` field in our state is used. As usual, the type checker is one step ahead of us, and is giving us an error pointing to <RepoItem repo=self.state.repoData /> which hints at the next change we need to make:

```
./src/index.re
Module build failed: Error: We've found a bug for you!
  /Users/jfriend/code/github-reason-list/src/app.re 22:22-36

  20 ┆   <div className="App">
  21 ┆     <h1>(ReasonReact.string("Reason Projects"))</h1>
  22 ┆     <RepoItem repo=self.state.repoData />
  23 ┆   </div>
  24 ┆ }

  This is:
    option(RepoData.repo)
  But somewhere wanted:
    RepoData.repo    at <anonymous>
```

Note that in the error output `self.state.repoData` is highlighted. We can't pass `self.state.repoData` directly as the repo prop of `RepoItem`, because it's wrapped in an option(), but RepoItem expects it without the option wrapper. So how do we unwrap it? We use pattern matching. This is where Reason forces use to cover all possible cases (or at least explicitly throw an error). Pattern matching makes use of the switch statement. Unlike a switch statement in Javascript however, the cases of a switch statement in Reason can match the types of the values (eg. Some and None), not just the values themselves. We'll change our render method to use a switch to provide logic to render our repo item in each possible case:

```
  render: self => {
    let repoItem =
      switch (self.state.repoData) {
      | Some(repo) => <RepoItem repo />
      | None => ReasonReact.string("Loading")
      };
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      repoItem
    </div>
  },
```

Here you can see the switch statement has a case to match a `self.state.repoData` value with the type `Some`, and pulls out the actual repo record into a variable called repo, which it then uses in the expression to the right of the `=>`, returning a `<RepoItem>` element. This expression will only be used in the case where self.state.repoData is `Some`. Alternatively, if self.state.repoData is `None`, the text "Loading" will be displayed instead.

## Reducer Components

So, why is the stateful component type in Reason React called `reducerComponent`? Reason React has a slightly different way of handling state changes in components as compared to JS React. If you've used Redux, it will look quite familiar. If you haven't, don't worry, no background knowledge is required here.

Basically instead of doing a bunch of stuff inside event handlers like `onClick` and then calling this.setState, we just figure out what kind of change we want to make to the component state, and call `self.send` with an 'action', which is just a value representing the kind of state change which should happen, along with any info we need to make the change. This means that most of the state changing code can be isolated in a pure function, making it easier to follow and much easier to write tests for.

We can try out making a state change in this way by making our state for repo initially `None` and then changing it once the user clicks on a button. This is a contrived example, but it's useful to illustrate state changes. Pretend we're loading data from the API when this button is clicked :).

Earlier, we added a type called action which enumerates the various kinds of possible state changes which could happen in our component. We initially created it with only a `PlaceholderAction`, because we weren't ready to discuss state yet, but now we can replace the placeholder with a `Loaded` action, for when the repo data is loaded:

```
type action =
 | Loaded(RepoData.repo);
```

After that we add a reducer method which takes one such action and the current state, then calculates and returns the updated state:

```
reducer: (action, _state) =>
    switch (action) {
    | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
    },
```

You can see that our implementation is pattern matching on the action type and returning a `ReasonReact.Update` which contains the new state. Right now we just have a case for the `Loaded` action, but in future we could conceivably have a other kinds of state changes implemented here, in response to different variants of action.

Next we change initialState, to start with no repo data:

```
initialState: () => {repoData: None},
```

Finally, we add the button in the render function. We use `self.send` to create a handler for the button's `onClick` prop. self.send takes a function which translates the click into an action for our reducer. A handler such as this might also use information from the click event object, but in this case we don't need it. We can create such a button like this:

```
let loadedReposButton =
  <button onClick=(_event => self.send(Loaded(dummyRepo)))>
    (ReasonReact.string("Load Repos"))
  </button>;
```

We can display this loadReposButton in place of the rendered RepoItem in the initial blank state (when self.state.repoData is `None`):

```
  render: self => {
    let loadedReposButton =
      <button onClick=(_event => self.send(Loaded(dummyRepo)))>
        (ReasonReact.string("Load Repos"))
      </button>;
    let repoItem =
      switch (self.state.repoData) {
      | Some(repo) => <RepoItem repo />
      | None => ReasonReact.string("Loading")
      | None => loadedReposButton
      };
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      repoItem
    </div>
  },
};
```

The extra step of using the action and reducer can seem overcomplicated when compared to calling `setState` in JS React, but as stateful components grow and have more possible states (with an increasing number of possible transitions between them) it's easy for the component to become a hard-to-follow and untestable tangle. This is where the action-reducer model really shines.

Okay, now we know how to do state changes, let's make this into a more realistic app.

## Arrays

Before we get into loading our data from JSON, there's one more change to make to the component. We actually want to show a list of repos, not just a single one, so we need to change the type of our state, and action:

```
type state = {repoData: option(array(RepoData.repo))};

type action =
  | Loaded(array(RepoData.repo));
```

And a corresponding change to our dummy data:

```
let dummyRepos: array(RepoData.repo) = [|
  {
    stargazers_count: 27,
    full_name: "jsdf/reason-react-hacker-news",
    html_url: "https://github.com/jsdf/reason-react-hacker-news"
  },
  {
    stargazers_count: 93,
    full_name: "reasonml/reason-tools",
    html_url: "https://github.com/reasonml/reason-tools"
  }
|];
```

Err, what's with the `[| ... |]` syntax? That's Reason's array literal syntax. If you didn't have the | pipe characters there (so it would look like the normal JS array syntax) then you would be defining a List instead of an array. In Reason lists are immutable, whereas arrays are mutable (like Javascript arrays), however lists are easier to work with if you are dealing with a variable number of elements. Anyway here we're using an array.

We'll need to go through the code and change all the places which refer to repoData as being RepoData.repo to instead specify array(RepoData.repo).

Finally, we'll change our render method to render an array of RepoItems instead of just one, by mapping over the array of repos and creating a `<RepoItem />` for each. We have to use `ReasonReact.array` to turn the array of elements into an element itself so it can be used in the JSX below.

```
  render: self => {
    let loadedReposButton =
      <button onClick=(_event => self.send(Loaded(dummyRepo)))>
        (ReasonReact.string("Load Repos"))
      </button>;
    let repoItems =
      | Some(repos) =>
        ReasonReact.array(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
            repos,
          ),
        )
      | None => loadedReposButton
    	};
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      repoItems
    </div>
   },
};
```

Now, to load some real data.

## BuckleScript

Before fetching our JSON and turning it into a record, first we need to install some extra dependencies. Run:

```
npm install --save bs-fetch @glennsl/bs-json
```

or

```
yarn add bs-fetch @glennsl/bs-json
```

Here's what these packages do:

bs-fetch: wraps the browser Fetch API so we can use it from Reason
@glennsls/bs-json: allows use to turn JSON fetched from the server into Reason records
These packages work with the Reason-to-JS compiler we've been using this whole time, which is called BuckleScript.

Before we can use these newly installed BuckleScript packages we need to let BuckleScript know about them. To do that we need to make some changes to the .bsconfig file in the root of our project. In the bs-dependencies section, add "bs-fetch" and "bs-json":

```
{
  "name": "reason-scripts",
  "sources": [
    "src"
  ],
  "bs-dependencies": [
    "reason-react",
    "bs-jest",
    "bs-fetch", // add this
    "@glennsl/bs-json" // and this too
  ],
  // ...more stuff
```

You'll need to kill and restart your yarn start/npm start command so that the build system can pick up the changes to .bsconfig.

## Reading JSON

Now we've installed `bs-json` we can use `Json.Decode` to read JSON and turn it into a record.

We'll define a function called `parseRepoJson` at the end of RepoData.re:

```
type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
};

let parseRepoJson = (json: Js.Json.t): repo => {
  full_name: Json.Decode.field("full_name", Json.Decode.string, json),
  stargazers_count: Json.Decode.field("stargazers_count", Json.Decode.int, json),
  html_url: Json.Decode.field("html_url", Json.Decode.string, json)
};
```

This defines a function called parseRepoJson which takes one argument called json and returns a value of the type RepoData.repo. The Json.Decode module provides a bunch of functions which we are composing together to extract the fields of the JSON, and assert that the values we're getting are of the correct type.

Don't repeat yourself
This is looking a bit wordy. Do we really have to write `Json.Decode` over and over again?

Nope, Reason has some handy syntax to help us when we need to refer to the exports of a particular module over and over again. One option is to 'open' the module, which means that all of its exports become available in the current scope, so we can ditch the Json.Decode qualifier:

```
open Json.Decode;

let parseRepoJson = (json: repo) =>
  {
    full_name: field("full_name", string, json),
    stargazers_count: field("stargazers_count", int, json),
    html_url: field("html_url", string, json)
  };
```

However this does introduce the risk of name collisions if you're opening multiple modules. Another option is to use the module name, followed by a period . before an expression. Inside the expression we can use any export of the module without qualifying it with the module name:

```
let parseRepoJson = (json: repo) =>
  Json.Decode.{
    full_name: field("full_name", string, json),
    stargazers_count: field("stargazers_count", int, json),
    html_url: field("html_url", string, json)
  };
```

Now let's test it out by adding some code which defines a string of JSON and uses our parseRepoJson function to parse it.

In App.re:

```
let dummyRepos: array(RepoData.repo) = [|
  RepoData.parseRepoJson(
    Js.Json.parseExn(
      {js|
        {
          "stargazers_count": 93,
          "full_name": "reasonml/reason-tools",
          "html_url": "https://github.com/reasonml/reason-tools"
        }
      |js}
    )
  )
|];
```

Don't worry about understanding what Js.Json.parseExn does or the weird {js| ... |js} thing (it's an alternative string literal syntax). Returning to the browser you should see the page successfully render from this JSON input.

## Fetching data

Looking at the form of the Github API response, we're interested in the items field. This field contains an array of repos. We'll add another function which uses our parseRepoJson function to parse the items field into an array of records.

In RepoData.re:

```
let parseReposResponseJson = json =>
  Json.Decode.field("items", Json.Decode.array(parseRepoJson), json);
```

Finally we'll make use of the bs-fetch package to make our HTTP request to the API.

But first, more new syntax! I promise this is the last bit. The pipe operator |> simply takes the result of the expression on the left of the |> operator and calls the function on the right of the |> operator with that value.

For example, instead of:

```
doThing3(doThing2(doThing1(arg)))
```

with the pipe operator we can do:

```
arg |> doThing1 |> doThing2 |> doThing3
```

This lets us simulate something like the chaining API of Promises in Javascript, except that `Js.Promise.then_` is a function we call with the promise as the argument, instead of being a method on the promise object.

In `RepoData.re`:

```
let reposUrl = "https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories";

let fetchRepos = () =>
  Bs_fetch.fetch(reposUrl)
    |> Js.Promise.then_(Bs_fetch.Response.text)
    |> Js.Promise.then_(
      jsonText =>
        Js.Promise.resolve(parseReposResponseJson(Js.Json.parseExn(jsonText)))
    );
```

We can make the Promise chaining fetchRepos a bit more terse by temporarily opening up Js.Promise:

```
let fetchRepos = () =>
  Js.Promise.(
    Bs_fetch.fetch(reposUrl)
      |> then_(Bs_fetch.Response.text)
      |> then_(
        jsonText =>
          resolve(parseReposResponseJson(Js.Json.parseExn(jsonText)))
      )
  );
```

Finally, back in `App.re` we'll add some code to load the data and store it in component state:

```
type state = {repoData: option(array(RepoData.repo))};

type action =
  | FetchRepos
  | ReposFailedToFetch(string)
  | Loaded(array(RepoData.repo));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  reducer: (action, _state) =>
    switch (action) {
    | FetchRepos =>
      ReasonReact.SideEffects(
        (
          self =>
            RepoData.fetchRepos()
            |> Js.Promise.then_(repoData => {
                 self.send(Loaded(repoData));
                 Js.Promise.resolve();
               })
            |> Js.Promise.catch(err =>
                 Js.Promise.resolve(
                   self.send(
                     ReposFailedToFetch(
                       "An error occurred: " ++ Js.String.make(err),
                     ),
                   ),
                 )
               )
            |> ignore
        ),
      )
    | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
    | ReposFailedToFetch(error) =>
      ReasonReact.SideEffects((_self => Js.log(error)))
    },
  didMount: self => self.send(FetchRepos),
  render: self => {
    let repoItems =
      switch (self.state.repoData) {
      | Some(repos) =>
        ReasonReact.array(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
            repos,
          ),
        )
      | None => ReasonReact.string("Loading")
      };
    <div className="App">
      <h1> (ReasonReact.string("Reason Projects")) </h1>
      repoItems
    </div>;
  },
};
```

First we implement the `didMount` lifecycle method. We use `self.send` to initiate the `FetchRepos` action to fetch our data. Note we are using `ReasonReact.SideEffects` because the `FetchRepos` action does not update the state, but has a side effect of loading data using our `RepoData.fetchRepos()` function, and calling the `Loaded(repo)` action to update the component state with the loaded data once it has been received. Much like promise chaining in Javascript, we pipe this into Js.Promise.then\_, where we call the handleReposLoaded function with the loaded data, updating the component state.

Note - we have removed the `loadReposButton` and reverted to the simple "Loading" placeholder while data is fetching, because the fetch request and subsequent state update is being handled on

We end the promise chain by returning Js.Promise.resolve(). We then implement a `catch` block for any errors. The whole expression defining the promise chain is then `|>` piped to a special function called `ignore`, which just tells Reason that we don't intend to do anything with the value that the promise chain expression evaluates to (we only care about the side effect it has of calling the updater function). Without this, you would get something akin to the following error message:

```
  This has type:
    Js.Promise.t(unit) (defined as Js.Promise.t(unit))
  But somewhere wanted:
    unit
```

And that's it!

You can see the completed app running here. The completed source is available on Github.

If you have any feedback about this article you can tweet me: @ur_friend_james

Hi! I'm James.
James Friend
I work at Facebook and make neat stuff on the web.
I'm into UX, music production, designing the future, and learning from the past.

Drop me a line
Email
Twitter
GitHub
LinkedIn

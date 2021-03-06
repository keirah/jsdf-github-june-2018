// Generated by BUCKLESCRIPT VERSION 3.1.5, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var RepoData$ReactTemplate = require("./RepoData.bs.js");
var RepoItem$ReactTemplate = require("./RepoItem.bs.js");

var component = ReasonReact.reducerComponent("App");

function make() {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              return Curry._1(self[/* send */3], /* FetchRepos */0);
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              var match = self[/* state */1][/* repoData */0];
              var repoItems = match ? $$Array.map((function (repo) {
                        return ReasonReact.element(/* Some */[repo[/* full_name */0]], /* None */0, RepoItem$ReactTemplate.make(repo, /* array */[]));
                      }), match[0]) : "Loading";
              return React.createElement("div", {
                          className: "App"
                        }, React.createElement("h1", undefined, "Reason Projects"), repoItems);
            }),
          /* initialState */(function () {
              return /* record */[/* repoData : None */0];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, _) {
              if (typeof action === "number") {
                return /* SideEffects */Block.__(1, [(function (self) {
                              RepoData$ReactTemplate.fetchRepos(/* () */0).then((function (repoData) {
                                        Curry._1(self[/* send */3], /* Loaded */Block.__(1, [repoData]));
                                        return Promise.resolve(/* () */0);
                                      })).catch((function (err) {
                                      return Promise.resolve(Curry._1(self[/* send */3], /* ReposFailedToFetch */Block.__(0, ["An error occurred: " + String(err)])));
                                    }));
                              return /* () */0;
                            })]);
              } else if (action.tag) {
                return /* Update */Block.__(0, [/* record */[/* repoData : Some */[action[0]]]]);
              } else {
                var error = action[0];
                return /* SideEffects */Block.__(1, [(function () {
                              console.log(error);
                              return /* () */0;
                            })]);
              }
            }),
          /* subscriptions */component[/* subscriptions */13],
          /* jsElementWrapped */component[/* jsElementWrapped */14]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */

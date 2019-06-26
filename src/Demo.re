// # Js.log("xxxHello, BuckleScript and Reason!");
// Open modules sparingly...
open Webapi.Dom;

// Abstract type for our Elm app
type app;

// JS object to pass into Elm.Main.init
[@bs.deriving abstract]
type elmArgs = {
  node: Dom.element,
  flags: string,
};

// Import our elm file:     var app = MainElm
// Scope it with "Elm":     var app = MainElm.Elm
// Get the "Main" property: var app = MainElm.Elm.Main
[@bs.scope "Elm"] [@bs.module "./elm/Main.elm"] external app: app = "Main";
// Do I want bs.send is used to call functions
// on a javascript object. When we pass in our abstract
// elm type we get: app.init(elmArgs)
[@bs.send] external init: (app, elmArgs) => elmArgs = "init";

let startElmApp = selector => {
  let element =
    document
    |> Document.querySelector(selector)
    |> (
      el =>
        // Unwrap the Optional dom element, there's probably a more graceful
        // way to do this, but for now it will suffice.
        switch (el) {
        | Some(e) => e
        | None => raise(Invalid_argument("Passed none to unrwap"))
        }
    );

  // Because elmArgs is also an abstract type (by some bs magic I don't
  // understand), We have to use the implictly created "creation function"
  // sort of like Elm type aliases.
  let initArgs = elmArgs(~node=element, ~flags="ReasonML");

  //  Finally call the init function of our Elm app and pray that
  // it all works!
  init(app, initArgs);
};

startElmApp("#app");

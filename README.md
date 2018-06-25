# jsdf-github-june-2018

## Run Project

```sh
npm install
npm start
# in another tab
npm run webpack
```

After you see the webpack compilation succeed (the `npm run webpack` step), open up `src/index.html` (**no server needed!**). Then modify whichever `.re` file in `src` and refresh the page to see the changes.

**For more elaborate ReasonReact examples**, please see https://github.com/reasonml-community/reason-react-example

## Run Alternative

## Add Parcel Bundler

`yarn add --dev parcel-bundler npm-run-all`

# Add index.html file

```html
<html>
<body>
  <div id="root"></div>
  <script src="./src/Index.bs.js"></script>
</body>
</html>
```

## Package.json Scripts

```json
  "scripts": {
    "build": "bsb -make-world",
    "dev": "npm-run-all --parallel start serve",
    "start": "bsb -make-world -w",
    "serve": "yarn parcel index.html",
    "clean": "bsb -clean-world"
  },
```

# Start Parcel in Browser

`yarn dev`

## Build for Production

```sh
npm run build
npm run webpack:production
```

This will replace the development artifact `build/Index.js` for an optimized version.

**To enable dead code elimination**, change `bsconfig.json`'s `package-specs` `module` from `"commonjs"` to `"es6"`. Then re-run the above 2 commands. This will allow Webpack to remove unused code.

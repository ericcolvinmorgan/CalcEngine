import { createBrowserHistory } from 'history';
import React, { useState } from 'react';
import { Route, Switch } from 'react-router';
import { BrowserRouter } from 'react-router-dom';
import useMediaQuery from '@material-ui/core/useMediaQuery';
import { createMuiTheme, ThemeProvider } from '@material-ui/core/styles';

import Main from './components/main'

function App() {
  const [lightMode, setLightMode] = useState(useMediaQuery('(prefers-color-scheme: light)'));
  
  const history = createBrowserHistory();
  const path = (/#!(\/.*)$/.exec(location.hash) || [])[1];
  if (path) {
      history.replace(path);
  }
  
  const theme = React.useMemo(
    () =>
      createMuiTheme({
        palette: {
          type: lightMode ? 'light' : 'dark',
          primary: {
            main: '#00897b'
          },
          secondary: {
            //main: lightMode ? '#fcc02a' : '#343a40'
            main: '#fcc02a'
          }
        },
      }),
    [lightMode],
  );

  return (
    <ThemeProvider theme={theme}>
      <BrowserRouter>
        <Switch>
          <Route exact path="/">
            <div>Welcome</div> 
          </Route>
          <Route path="/testing" render={(props) => <Main {...props} lightMode={lightMode} setLightMode={setLightMode} />}  />
        </Switch>
      </BrowserRouter>
    </ThemeProvider>
  );
}

export default App;

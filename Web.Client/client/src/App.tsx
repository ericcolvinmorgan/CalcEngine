import { createBrowserHistory } from 'history';
import React, { useState, useReducer } from 'react';
import { Route, Switch } from 'react-router';
import { BrowserRouter } from 'react-router-dom';
import useMediaQuery from '@material-ui/core/useMediaQuery';
import { createMuiTheme, ThemeProvider } from '@material-ui/core/styles';
import { useCalculationsContext } from './providers/calculationsProvider'
import { CalculationsReducer } from './reducers/calculationsReducer';
import { CalculationsProvider } from './providers/calculationsProvider';
import Main from './components/main'

function App() {
  const [lightMode, setLightMode] = useState(useMediaQuery('(prefers-color-scheme: light)'));
  
  const history = createBrowserHistory();
  const path = (/#!(\/.*)$/.exec(window.location.hash) || [])[1];
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

  const initialState = {
    items: []
  }
  
  const pathTest = process.env.REACT_APP_WEBAPI_CALCULATIONS_ENDPOINT;
  const reducer = useReducer(CalculationsReducer, initialState)
  const {state} = useCalculationsContext();

  return (
    <ThemeProvider theme={theme}>
      <BrowserRouter>
        <Switch>
          <Route exact path="/">
            <div>Welcome - {pathTest}</div> 
          </Route>
          <CalculationsProvider value={reducer}>          
          <Route path="/portal" render={(props) => <Main {...props} lightMode={lightMode} setLightMode={setLightMode} />}  />
          </CalculationsProvider>
        </Switch>
      </BrowserRouter>
    </ThemeProvider>
  );
}

export default App;

//import './../scss/main.scss'
import * as React from 'react';
import { AiOutlineMenuUnfold } from 'react-icons/ai'
import { MdMoveToInbox as InboxIcon, MdMail as MailIcon, MdMenu as MenuIcon, MdChevronLeft as ChevronLeftIcon, MdChevronRight as ChevronRightIcon } from 'react-icons/md'
import { BsGearFill } from 'react-icons/bs'
import { FiCpu } from 'react-icons/fi'
import { Link } from 'react-router-dom';
import { useState } from 'react';
import { Route, Switch as RouterSwitch, } from 'react-router';
import { createStyles, makeStyles, useTheme, Theme } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import IconButton from '@material-ui/core/IconButton';
import Switch from '@material-ui/core/Switch';
import Drawer from '@material-ui/core/Drawer';
import List from '@material-ui/core/List';
import CssBaseline from '@material-ui/core/CssBaseline';
import Divider from '@material-ui/core/Divider';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import clsx from 'clsx';

import BlockManager from './blockmanager';
import Calculations from './calculations'

const drawerWidth = 240
const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    root: {
      display: 'flex',
    },
    appBar: {
      zIndex: theme.zIndex.drawer + 1,
      transition: theme.transitions.create(['width', 'margin'], {
        easing: theme.transitions.easing.sharp,
        duration: theme.transitions.duration.leavingScreen,
      })
    },
    appBarShift: {
      marginLeft: drawerWidth,
      width: `calc(100% - ${drawerWidth}px)`,
      transition: theme.transitions.create(['width', 'margin'], {
        easing: theme.transitions.easing.sharp,
        duration: theme.transitions.duration.enteringScreen,
      }),
    },
    menuButton: {
      marginRight: 36,
    },
    hide: {
      display: 'none',
    },
    drawer: {
      width: drawerWidth,
      flexShrink: 0,
      whiteSpace: 'nowrap',
    },
    drawerOpen: {
      width: drawerWidth,
      transition: theme.transitions.create('width', {
        easing: theme.transitions.easing.sharp,
        duration: theme.transitions.duration.enteringScreen,
      }),
    },
    drawerClose: {
      transition: theme.transitions.create('width', {
        easing: theme.transitions.easing.sharp,
        duration: theme.transitions.duration.leavingScreen,
      }),
      overflowX: 'hidden',
      width: theme.spacing(7) + 1,
      [theme.breakpoints.up('sm')]: {
        width: theme.spacing(9),
      },
    },
    paper: {
      backgroundColor: theme.palette.type === 'dark' ? theme.palette.grey[700] : theme.palette.primary.main,
      color: '#fff',
      borderStyle: 'none'
    },
    menuItemIcon: {
      color: 'inherit',
      paddingLeft: '13px'
    },
    menuItem: {
      '&:hover': { color: theme.palette.secondary.main }
    },
    toolbar: {
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'flex-end',
      padding: theme.spacing(0, 1),
      // necessary for content to be below app bar
      ...theme.mixins.toolbar,
    },
    toolbarHeader: {
      backgroundColor: theme.palette.primary.main
    },
    content: {
      flexGrow: 1,
      padding: theme.spacing(3),
    },
    grow: {
      flexGrow: 1,
    }
  }),
);

const Main = ({ match, lightMode, setLightMode }: any) => {
  const classes = useStyles();
  const theme = useTheme();
  const [open, setOpen] = React.useState(false);

  const handleDrawerOpen = () => {
    setOpen(true);
  };

  const handleDrawerClose = () => {
    setOpen(false);
  };

  return (
    <div className={classes.root}>
      <CssBaseline />
      <AppBar
        position="fixed"
        color="primary"
        className={clsx(classes.appBar, {
          [classes.appBarShift]: open,
        })}
      >
        <Toolbar>
          <Button variant="contained"
            color="secondary"
            size="large"
            aria-label="open drawer"
            onClick={handleDrawerOpen}
            className={clsx(classes.menuButton, {
              [classes.hide]: open,
            })}
          >
            <AiOutlineMenuUnfold />
          </Button>
          <Typography variant="h6" noWrap>
            Calc Engine
          </Typography>
          <div className={classes.grow} />
          <div>
            <Switch
              checked={!lightMode}
              color="default"
              onChange={() => setLightMode(!lightMode)}
              name="checkedB"
              inputProps={{ 'aria-label': 'primary checkbox' }}
            />
          </div>
        </Toolbar>
      </AppBar>
      <Drawer
        variant="permanent"
        className={clsx(classes.drawer, {
          [classes.drawerOpen]: open,
          [classes.drawerClose]: !open,
        })}
        classes={{
          paper: clsx(classes.paper, {
            [classes.drawerOpen]: open,
            [classes.drawerClose]: !open,
          }),
        }}
      >
        <div className={clsx(classes.toolbar, classes.toolbarHeader)}>
          <IconButton onClick={handleDrawerClose}>
            {theme.direction === 'rtl' ? <ChevronRightIcon /> : <ChevronLeftIcon />}
          </IconButton>
        </div>
        <Divider />
        <List>
          <Link to={match.path + "/blockmanager"} style={{ textDecoration: 'none', color: 'inherit' }}>
            <ListItem button className={classes.menuItem}>
              <ListItemIcon className={classes.menuItemIcon}><BsGearFill /></ListItemIcon>
              <ListItemText primary={"Block Manager"} />
            </ListItem>
          </Link>
          <Link to={match.path + "/calculations"} style={{ textDecoration: 'none', color: 'inherit' }}>
          <ListItem button className={classes.menuItem}>
            <ListItemIcon className={classes.menuItemIcon}><FiCpu /></ListItemIcon>
            <ListItemText primary={"Calculations"} />
          </ListItem>
          </Link>
        </List>
      </Drawer>
      <main className={classes.content}>
        <div className={classes.toolbar} />
        <RouterSwitch>
          <Route exact path="/">
            <div>Welcome - <Link to={match.path + "/blockmanager"} style={{ textDecoration: 'none', color: 'inherit' }}></Link></div> 
          </Route>
          <Route path={match.path + "/blockmanager"} component={BlockManager}  />
        </RouterSwitch>
      </main>
    </div>
  )
}

export default Main

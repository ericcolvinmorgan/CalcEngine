import React, {useState} from 'react'
import { makeStyles, Theme, createStyles } from '@material-ui/core/styles';
import { MdAdd } from 'react-icons/md';
import CalculationsList from './calculationslist'
import { Fab, Container } from '@material-ui/core';
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';
import Input from '@material-ui/core/Input';
import { useGetCalculations  } from '../hooks/useCalculationsApi'
import { postCalculationRequest } from '../api/calculationsApi';

const useStyles = makeStyles((theme: Theme) =>
    createStyles({
        root: {
            width: '100%',
        },
        heading: {
            fontSize: theme.typography.pxToRem(15),
            flexBasis: '33.33%',
            flexShrink: 0,
        },
        secondaryHeading: {
            fontSize: theme.typography.pxToRem(15),
            color: theme.palette.text.secondary,
        },
        fab: {
            position: 'absolute',
            bottom: theme.spacing(2),
            right: theme.spacing(2),
        },
    }),
);

const Calculations = () => {
    const [selectedFile, setSelectedFile] = useState<any>(null);
    const [state, , load] = useGetCalculations();
    const classes = useStyles();
    const [expanded, setExpanded] = React.useState<string | false>(false);
    const [uploadResult, setUploadResult] = useState({});
    const [open, setOpen] = React.useState(false);

    const handleClickOpen = () => {
        setOpen(true);
    };

    const onSelectFile = (e:any) => {
        const file = e.target.files[0];
        setSelectedFile(file)
    }

    const handleClose = () => {
        setOpen(false);
    };

    const handleFileSubmit = async () =>
    {
        const response = await postCalculationRequest({'name': selectedFile.name});
        console.log('state');
    }

    const handleChange = (panel: string) => (event: React.ChangeEvent<{}>, isExpanded: boolean) => {
        setExpanded(isExpanded ? panel : false);
    };

    const fileData = () => { 
     
        if (selectedFile) { 
            
          return ( 
            <div> 
              <h2>File Details:</h2> 
              <p>File Name: {selectedFile.name}</p> 
              <p>File Type: {selectedFile.type}</p> 
              <p> 
                Last Modified:{" "} 
                {selectedFile.lastModifiedDate.toDateString()} 
              </p> 
            </div> 
          ); 
        } else { 
          return ( 
            <div> 
              <br /> 
              <h4>Choose before Pressing the Upload button</h4> 
            </div> 
          ); 
        } 
      }; 

    return (
        <div className={classes.root}>
            <Container maxWidth="lg">
                <CalculationsList></CalculationsList>
            </Container>

            <Fab className={classes.fab} color='secondary' onClick={handleClickOpen}>
                <MdAdd />
            </Fab>

            <Dialog open={open} onClose={handleClose} aria-labelledby="form-dialog-title">
                <DialogTitle id="form-dialog-title">Request Calculation</DialogTitle>
                <DialogContent>
                    <DialogContentText>
                        Please select a valid calculation *.CSV file to upload to the server.
                      </DialogContentText>
                    <Input
                        autoFocus
                        margin="dense"
                        id="raised-button-file"
                        type="file"
                        style={{display:'none'}}
                        onChange={onSelectFile}                                                                  
                    />
                     <label htmlFor="raised-button-file"> 
                    <Button component="span"> 
                        Select File 
                    </Button>
                    <div>
                        {fileData()}
                    </div>
                    </label> 
                </DialogContent>
                <DialogActions>
                    <Button onClick={handleClose} color="primary">
                        Cancel
          </Button>
                    <Button onClick={handleFileSubmit} color="primary">
                        Upload
          </Button>
                </DialogActions>
            </Dialog>
        </div>
    )
}

export default Calculations;
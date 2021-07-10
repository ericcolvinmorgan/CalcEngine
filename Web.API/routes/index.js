var express = require('express');
var router = express.Router();
const ExcelParser = require('../services/excelParser.js');

let requests = 0;
/* GET home page. */
router.get('/', function(req, res, next) {

  const parser = new ExcelParser('../ExcelParser.Console/Basic1.xlsx', './output.json');
  parser.process().then(() => {
    if(parser.isError)
    {
      console.log(parser.errorMessage);
    }
    else
    {
      console.log(JSON.stringify(parser.output));
    }
  });

  res.send(`respond with a resource ${++requests}`);
});

module.exports = router;

const PROCESS_LOCATION = process.env.EXCEL_PARSER_LOCATION;

const { exec } = require('child_process');

class ExcelParser {
    _filePath;
    _outputPath;
    isError = false;
    errorMessage = '';
    output;

    constructor(filePath, outputPath) {
        this._filePath = filePath;
        this._outputPath = outputPath;
    }

    async process() {
        return new Promise((resolve) => {
            const cmd = exec(`${PROCESS_LOCATION} ${this._filePath} -o ${this._outputPath}`, (err, stdout, stderr) => {
                if (err) {
                    this.isError = true;
                    this.errorMessage = err;
                    resolve();
                }

                if (stderr) {
                    this.isError = true;
                    this.errorMessage = stderr;
                    resolve();
                }

                this.output = stdout;
                resolve();
            });
        });
    }
}

module.exports = ExcelParser;
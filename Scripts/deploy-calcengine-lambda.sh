echo "Creating $1"
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$1

echo "Compiling $1"
make

#./CalcEngine.Console/CalcEngine.Console /mnt/c/Users/ericm/Documents/source/repos/CalcEngine/ExcelParser.Tests/TestFiles/TestFile.json /mnt/c/Users/ericm/TestingFiles/EPDataTypes.csv /mnt/c/Users/ericm/TestingFiles/EPValues.csv

if [ $2 = 'Deploy' ]
    then 
        echo "Deploying CalcEngine.Lambda $1"
        make aws-lambda-package-CalcEngine.Lambda
        aws lambda update-function-code \
            --function-name "arn:aws:lambda:us-west-2:542100369852:function:calc-engine" \
            --zip-file fileb://./CalcEngine.Lambda/CalcEngine.Lambda.zip

        aws lambda invoke --function-name calc-engine --payload '{"s3bucket": "calc-build-calc-definitions", "s3jsonkey":"TestFile.json", "s3datatypeskey":"EPDataTypes.csv", "s3valueskey":"EPValues.csv" }' --cli-binary-format raw-in-base64-out ../.testoutput/output.txt && cat ../.testoutput/output.txt
fi
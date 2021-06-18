export const getCalculations = () => {
    const url = process.env.REACT_APP_WEBAPI_CALCULATIONS_ENDPOINT as string;
    let options:RequestInit = {}
    return fetch(url, options)
}

export const postCalculationRequest = (postData : any) => {
    const url = process.env.REACT_APP_WEBAPI_CALCULATION_REQUEST_ENDPOINT as string;
    let options:RequestInit = {
        method: 'POST',
        headers: new Headers({'content-type': 'application/json'}),
        body: JSON.stringify(postData)
    }
    return fetch(url, options)
}
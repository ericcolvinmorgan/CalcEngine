import { useEffect, useState } from 'react'
import { getCalculations, postCalculationRequest } from '../api/calculationsApi';

export const useGetCalculations = (): [any, any, boolean] => {    
    const [data, setData] = useState({})    
    const [error, setError] = useState('');
    const [loading, setLoading] = useState(true);
    useEffect(() => {
        const callApi = async () => {
            try {
                const response = await getCalculations();
                const responseData = await response.json();
                setData(responseData);
                setLoading(false);
            }
            catch (error)
            {
                console.log(error);
                setError(error);
            }
        }
        callApi();
    }, []);
    return [data, error, loading];
}

export const usePostCalculationRequest = (postData : any): [any, any, boolean] => {    
    const [data, setData] = useState({})    
    const [error, setError] = useState('');
    const [loading, setLoading] = useState(true);
    useEffect(() => {
        const callApi = async () => {
            try {
                //Create Initial Request
                const response = await postCalculationRequest(postData);
                const responseData = await response.json();
                setData(responseData);
                setLoading(false);
            }
            catch (error)
            {
                console.log(error);
                setError(error);
            }
        }
        callApi();
    }, [postData]);
    return [data, error, loading];
}
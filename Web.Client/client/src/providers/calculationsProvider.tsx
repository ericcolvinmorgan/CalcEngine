import {createContext, useContext} from 'react'

export const CalculationsContext = createContext<[any, React.Dispatch<any> | any]>([{}, () => {} ]);
export const CalculationsProvider = CalculationsContext.Provider;

export const useCalculationsContext = () => {
    const calculationsContext = useContext(CalculationsContext);
    return {
        state: calculationsContext[0],
        dispatch: calculationsContext[1]
    }
}
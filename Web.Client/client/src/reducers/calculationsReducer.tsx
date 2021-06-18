import { CalculationsState } from '../models/calculationsState'
export const CalculationsReducer = (state: CalculationsState, action: {type: string, item: any}): CalculationsState => {
  switch (action.type) {
    case 'ADD':
      return {
        items: [...state.items, {
          ...action.item
        }]
      };
    default:
      throw new Error('Unexpected Calculations Action');
  }
};
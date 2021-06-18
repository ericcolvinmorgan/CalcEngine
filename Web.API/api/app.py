from chalice import Chalice, CORSConfig 
import boto3
from botocore.exceptions import ClientError
import os
import psycopg2
import uuid

app = Chalice(app_name='api')

# Connect to an existing database
# Relies on following environment variables (set in venv activate file for dev)
# export PGHOST=*
# export PGDATABASE=*
# export PGUSER=*
# export PGPASSWORD=*
conn = psycopg2.connect('')

cors_config = CORSConfig(
    allow_origin='*'
)

@app.route('/calculations', cors=cors_config)
def get_calculations():    
    try:
        cur = conn.cursor()
        cur.execute("""SELECT calculation_id, calculation_block_id, calculation_block_datatype_id, status, requested_on, requested_by, status_updated_on FROM public.calculations;""")
        calcs = cur.fetchall()
        return { 'success': True, 'values': [{'item': 1}, {'item': 2}] }

    except:
        return { 'success': False, 'message': 'Invalid request.' }

@app.route('/calculations/request', methods=['POST'], cors=cors_config)
def request_calculation():    
    try:
        #Query to create calculation record.
        calculation_id = str(uuid.uuid4())
        request = app.current_request.json_body
        insert_query = """INSERT INTO public.calculations(
	        calculation_id, calculation_block_id, calculation_block_datatype_id, status, requested_on, requested_by, status_updated_on)
	        VALUES (%s, %s, %s, %s, timezone('utc'::text, now()), %s, timezone('utc'::text, now()));"""
        insert_values = (calculation_id, request.block_id, request.datatype_id, 0, 'b49c372f-f508-42e9-9621-3a8f1142015d')
        cur = conn.cursor()
        cur.execute(insert_query, insert_values)
        conn.commit()

        s3_client = boto3.client('s3')
        key = s3_client.generate_presigned_post('calc-build-calc-definitions', calculation_id, Fields=None, Conditions=None, ExpiresIn=3600)
    
    except ClientError:
        return { 'success': False, 'message': 'Could not generate upload key.' } 
    except:
        return { 'success': False, 'message': 'Invalid request.' }
    
    return { 'success': True, 'calculation_id': calculation_id, 'upload_key': key }

@app.route('/hello/{name}')
def hello_name(name):    
    return {'hello!': name}
#
# @app.route('/users', methods=['POST'])
# def create_user():
#     # This is the JSON body the user sent in their POST request.
#     user_as_json = app.current_request.json_body
#     # We'll echo the json body back to the user in a 'user' key.
#     return {'user': user_as_json}
#
# See the README documentation for more examples.
#


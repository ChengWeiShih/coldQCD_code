import numpy as np

import psycopg2 # note : the interface of the psql and python
import psycopg2.extras
import pandas as pd

import matplotlib.pyplot as plt

# import glob, sys, fitz # note : for converting the pdf file to png, run "pip install PyMuPDF" prior to the compilation
# import matplotlib.pyplot as plt
# import matplotlib.image as img

import time

def initialize_psql_server(hostname, database, username, pwd, port_id):
    try : 
        conn = psycopg2.connect (host = hostname, dbname = database, user = username, password = pwd, port = port_id) 
        # cur = conn.cursor()
        cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
        return cur, conn
    except Exception as error : 
        print(error)

def show_cali_table(cur,conn, run_script) : 
    
    cur.execute(run_script)
    colnames = [desc[0] for desc in cur.description]
    df = pd.DataFrame(data = cur.fetchall(), columns = colnames)
    # print(df)
    # print(df.to_string())


    conn.commit()

    return df

daq_hostname = 'sphnxdaqdbreplica1.sdcc.bnl.gov'
daq_database = 'daq'
daq_username = 'phnxrc'
daq_pwd      = 'admin_test' # note : not pretty sure it is correct or not.
daq_port_id  = '5432'

run_duration_cut = 0
run_runnumber_cut = 1
run_year_cut = 2024
mbd_index = 10 # note : MBD N&S >= 1
run_type_cut = 'physics'

run_mbdevt_script = '''
    SELECT *                  
    FROM (
        SELECT 
            run.runnumber, 
            run.runtype, 
            run.brtimestamp, 
            run.ertimestamp, 
            ROUND(EXTRACT(EPOCH FROM (run.ertimestamp - run.brtimestamp)) / 60, 3) AS duration_minutes, 
            gl1_scalers.index, 
            gl1_scalers.raw,
            gl1_scalers.live,
            gl1_scalers.scaled
        FROM run 
        INNER JOIN gl1_scalers ON run.runnumber = gl1_scalers.runnumber 
        WHERE EXTRACT(YEAR FROM run.brtimestamp) = %i 
        AND run.runtype = '%s' 
        AND run.runnumber > %i 
        AND gl1_scalers.index = %i
        AND EXTRACT(YEAR FROM run.ertimestamp) = %i
    ) AS subquery
    WHERE subquery.duration_minutes > %i  -- Example condition
    ORDER BY subquery.runnumber;
'''%(run_year_cut, run_type_cut, run_runnumber_cut, mbd_index, run_year_cut, run_duration_cut)

daq_cur,daq_conn = initialize_psql_server(daq_hostname, daq_database, daq_username, daq_pwd, daq_port_id)

run_mbdevt_df = show_cali_table(daq_cur,daq_conn, run_mbdevt_script)

print("total runs: ", run_mbdevt_df.shape[0])
print("runs greater than 10 mins: ", run_mbdevt_df[run_mbdevt_df['duration_minutes'] > 10].shape[0])
print("runs greater than 15 mins: ", run_mbdevt_df[run_mbdevt_df['duration_minutes'] > 15].shape[0])
print("runs greater than 20 mins: ", run_mbdevt_df[run_mbdevt_df['duration_minutes'] > 20].shape[0])

plt.hist(run_mbdevt_df['duration_minutes'], bins=50, range=(-10, 80), edgecolor='black')
plt.axvline(x=20, color='green', linestyle='--', label='20 mins line')
plt.xlabel('run duration [minutes]')
plt.ylabel('Entry')
plt.title('Run duration distribution')
plt.legend()
plt.show()
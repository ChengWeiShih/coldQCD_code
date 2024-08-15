import numpy as np

import psycopg2 # note : the interface of the psql and python
import psycopg2.extras
import pandas as pd

import matplotlib.pyplot as plt

import argparse

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

run_duration_cut = 10
run_runnumber_cut = 1
run_year_cut = 2024
mbd_index = 10 # note : MBD N&S >= 1
ZDC_index = 3 # note : ZDCNS coincidence
run_type_cut = 'physics'
polarization_cut = 25 # note : 25% polarization cut
N_ZDCNS_evt_cut = 500
GL1P_runnumber = 45235 # note : June 9th 2024, 2:00 AM, according to Devon from Mattermost

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

run_ZDCcoinevt_script = '''
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
'''%(run_year_cut, run_type_cut, run_runnumber_cut, ZDC_index, run_year_cut, run_duration_cut)

Fill_hostname = 'sphnxdbmaster.sdcc.bnl.gov'
Fill_database = 'spinDB'
Fill_username = 'phnxrc'
Fill_pwd      = 'admin_test' # note : not pretty sure it is correct or not.
Fill_port_id  = '5432'
Fill_run_script = '''
    select runnumber,fillnumber,polarblue[1],polarblueerror[1],polaryellow[1],polaryellowerror[1] from spin order by runnumber;
'''

def main():

    parser = argparse.ArgumentParser(description='Example script with command-line arguments.')

    # note : Add arguments
    parser.add_argument('--GL1P', action='store_true', help='only check the runs after the GL1P became available or not')
    # parser.add_argument('age', type=int, help='Your age')
    parser.add_argument('--BeamPolarPlot', action='store_true', help='Make the beam polarization plot')
    parser.add_argument('--ZDCNSEventPlot', action='store_true', help='Make the ZDCNS event plot')

    # note : Parse the arguments
    args = parser.parse_args()


    # note : Initialize the psql server
    daq_cur,daq_conn = initialize_psql_server(daq_hostname, daq_database, daq_username, daq_pwd, daq_port_id)

    # note : grabe the table from the psql server, and make the dataframe
    run_mbdevt_df = show_cali_table(daq_cur,daq_conn, run_mbdevt_script)
    
    if args.GL1P:
        run_mbdevt_df.query('runnumber >= %i'%GL1P_runnumber, inplace=True)

    run_mbdevt_df['scaled_cumulative_sum'] = run_mbdevt_df['scaled'].cumsum()
    run_mbdevt_df = run_mbdevt_df.rename(columns={'scaled': 'MBD_scaled'})
    run_mbdevt_df['brtimestamp'] = pd.to_datetime(run_mbdevt_df['brtimestamp'])

    # note : grab the ZDCNS coincidence event table from the psql server, and make the dataframe
    run_ZDCevt_df = show_cali_table(daq_cur,daq_conn, run_ZDCcoinevt_script)
    
    if args.GL1P:
        run_ZDCevt_df.query('runnumber >= %i'%GL1P_runnumber, inplace=True)

    run_ZDCevt_df['scaled_cumulative_sum'] = run_ZDCevt_df['scaled'].cumsum()
    run_ZDCevt_df = run_ZDCevt_df.rename(columns={'scaled': 'ZDCNS_scaled'})
    run_ZDCevt_df['brtimestamp'] = pd.to_datetime(run_ZDCevt_df['brtimestamp'])
    run_ZDCevt_df['ZDC_scaled_rate'] = (run_ZDCevt_df['ZDCNS_scaled'].astype(float) / (run_ZDCevt_df['duration_minutes'].astype(float) * 60.0)) # note : Hz

    # print(run_ZDCevt_df_GL1P.to_string())

    # note : Initialize the psql server
    Fill_cur,Fill_conn = initialize_psql_server(Fill_hostname, Fill_database, Fill_username, Fill_pwd, Fill_port_id)
    # note : grabe the table from the psql server, and make the dataframe
    Fill_df = show_cali_table(Fill_cur,Fill_conn, Fill_run_script)
    # print(Fill_df)

    run_ZDCNSevt_fill_combined_df = pd.merge(run_ZDCevt_df, Fill_df, on='runnumber', how='inner')
    run_ZDCNSevt_MBD_fill_combined_dfmerged_df = pd.merge(run_ZDCNSevt_fill_combined_df, run_mbdevt_df[['runnumber', 'MBD_scaled']], on='runnumber', how='left')
    # print(run_ZDCNSevt_MBD_fill_combined_dfmerged_df.to_string())

    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut = run_ZDCNSevt_MBD_fill_combined_dfmerged_df.copy()
    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut.query('polarblue > %i and polaryellow > %i'%(polarization_cut,polarization_cut), inplace=True)
    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['MBD_scaled_cumulative_sum'] = run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['MBD_scaled'].cumsum()
    print(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut.to_string())

    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut = run_ZDCNSevt_MBD_fill_combined_dfmerged_df.copy()
    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut.query('polarblue > %i and polaryellow > %i and ZDCNS_scaled > %i'%(polarization_cut,polarization_cut,N_ZDCNS_evt_cut), inplace=True)
    if not args.GL1P:
        run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut.query('runnumber >= %i'%GL1P_runnumber, inplace=True)

    run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['MBD_scaled_cumulative_sum'] = run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['MBD_scaled'].cumsum()

    daily_run_mbdevt_df = run_mbdevt_df[['brtimestamp','MBD_scaled']].copy()
    daily_run_mbdevt_df['date'] = daily_run_mbdevt_df['brtimestamp'].dt.date
    daily_run_mbdevt_df = daily_run_mbdevt_df.groupby('date')['MBD_scaled'].sum().reset_index()
    daily_run_mbdevt_df.rename(columns={'MBD_scaled': 'full_MBD_scaled'}, inplace=True)

    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut = run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut[['brtimestamp','MBD_scaled']].copy()
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['date'] = daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['brtimestamp'].dt.date
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut = daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut.groupby('date')['MBD_scaled'].sum().reset_index()
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut.rename(columns={'MBD_scaled': 'beampola_MBD_scaled'}, inplace=True)

    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut = run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut[['brtimestamp','MBD_scaled']].copy()
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['date'] = daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['brtimestamp'].dt.date
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut = daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut.groupby('date')['MBD_scaled'].sum().reset_index()
    daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut.rename(columns={'MBD_scaled': 'beampolaZDC_MBD_scaled'}, inplace=True)

    daily_merged_df = pd.merge(daily_run_mbdevt_df, daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut, on='date', how='outer')
    daily_merged_df = pd.merge(daily_merged_df, daily_run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut, on='date', how='outer')
    daily_merged_df.fillna(0, inplace=True)


    daily_merged_df['full_MBD_scaled_sum'] = daily_merged_df['full_MBD_scaled'].cumsum()
    daily_merged_df['beampola_MBD_scaled_sum'] = daily_merged_df['beampola_MBD_scaled'].cumsum()
    daily_merged_df['beampolaZDC_MBD_scaled_sum'] = daily_merged_df['beampolaZDC_MBD_scaled'].cumsum()


    # print(daily_merged_df.to_string())


    # print(run_ZDCNSevt_MBD_fill_combined_dfmerged_df.to_string())
    # print(run_mbdevt_df.to_string())

    # plt.figure(figsize=(10, 5))
    # plt.scatter(run_mbdevt_df['brtimestamp'], run_mbdevt_df['scaled_cumulative_sum'], label='Total MBD events', color = 'blue')
    # plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['MBD_scaled_cumulative_sum'], label='(Rough) spin total MBD events, polar_cut', color = 'red')
    # plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['MBD_scaled_cumulative_sum'], label='(Rough) spin total MBD events, polar_cut + nZDCNS_cut', color = 'orange')
    # plt.xlabel('brtimestamp')
    # plt.ylabel('MBD N&S >= 1 scaled event counting')
    # plt.title('brtimestamp vs. MBD N&S >= 1 scaled event counting')
    # plt.xticks(rotation=45)
    # plt.legend()
    # plt.tight_layout()
    # plt.show()

    fig = plt.figure(figsize=(12, 8))
    gs = fig.add_gridspec(2, 1, height_ratios=[3, 1], hspace=0.05)
    # note : Create the main plot (top subplot)
    ax_main = fig.add_subplot(gs[0])
    ax_main.scatter(run_mbdevt_df['brtimestamp'], run_mbdevt_df['scaled_cumulative_sum'], label='Total MBD events', color = 'blue', s=10)
    ax_main.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['MBD_scaled_cumulative_sum'], label='(Rough) spin total MBD events, polar_cut', color = 'red', s=10)
    ax_main.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut_ZDCNScut['MBD_scaled_cumulative_sum'], label='(Rough) spin total MBD events, polar_cut + nZDCNS_cut', color = 'orange', s=10)
    ax_main.set_ylabel('MBD N&S >= 1 scaled event counting')
    ax_main.legend(loc='upper left')
    plt.xticks(rotation=35)
    ax_main.grid(True)

    # note : Create the ratio plot (bottom subplot)
    ax_ratio = fig.add_subplot(gs[1])
    # ax_ratio.axhline(y=1, color='g',linestyle='--')
    # ax_ratio.axhline(y=0, color='g',linestyle='--')
    ax_ratio.plot(daily_merged_df['date'], daily_merged_df['beampola_MBD_scaled_sum']/daily_merged_df['full_MBD_scaled_sum'], 'o-', color='red', markersize=3.5)
    ax_ratio.plot(daily_merged_df['date'], daily_merged_df['beampolaZDC_MBD_scaled_sum']/daily_merged_df['full_MBD_scaled_sum'], 'o-', color='orange', markersize=3.5)
    ax_ratio.set_xlabel('date')
    ax_ratio.set_ylabel('Ratio w.r.t. full MBD statistics')
    ax_ratio.set_ylim([-0.1,1.1])
    ax_ratio.set_xlim(ax_main.get_xlim())
    plt.xticks(rotation=35)
    ax_ratio.grid(True)

    # note : Hide x labels for the top plot
    ax_main.set_xticklabels([])
    # note : Show the plot
    plt.show()



    if args.BeamPolarPlot:
        # note : beam polarization
        plt.figure(figsize=(10, 5))
        plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df['polarblue'], label='Blue line polarization', color = 'blue', s=10)
        plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df['polaryellow'], label='Yellow line polarization', color = 'orange', s=10)
        plt.axhline(y=100, color='r', linestyle='--', label='100%% line')
        plt.axhline(y=50, color='black', linestyle='--', label='50%% line')
        plt.axhline(y=0, color='g', linestyle='--', label='0%% line')
        plt.xlabel('brtimestamp')
        plt.ylabel('Beam polarization [%]')
        plt.title('Beam polarization vs. time')
        plt.ylim(-10,110)
        plt.xticks(rotation=45)
        plt.legend()
        plt.tight_layout()
        plt.show()

    if args.ZDCNSEventPlot:
        # note : N_ZDCNS_event
        plt.figure(figsize=(10, 5))
        plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['runnumber'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df_polarcut['ZDCNS_scaled'], label='ZDCNS N events', color = 'red', s=10)
        plt.axhline(y=100000, color='green', linestyle='--', label='Nevent = 100k')
        plt.axhline(y=10000, color='orange', linestyle='--', label='Nevent = 10k')
        plt.axhline(y=500, color='brown', linestyle='--', label='Nevent = 0.5k')
        plt.xlabel('runID')
        plt.ylabel('N ZDCNS event')
        plt.title('N ZDCNS event vs. runID')
        plt.xticks(rotation=45)
        plt.legend()
        plt.tight_layout()
        plt.ylim(1, 50000000)
        plt.yscale('log')
        plt.grid(True)
        plt.show()

    # plt.figure(figsize=(10, 5))
    # plt.scatter(run_ZDCNSevt_MBD_fill_combined_dfmerged_df['brtimestamp'], run_ZDCNSevt_MBD_fill_combined_dfmerged_df['ZDC_scaled_rate'], label='ZDC scaled trigger rate', color = 'orange', s=15)
    # plt.axhline(y=200, color='r', linestyle='--', label='200 Hz line')
    # plt.axhline(y=100, color='b', linestyle='--', label='100 Hz line')
    # plt.axhline(y=50, color='g', linestyle='--', label='50 Hz line')
    # plt.xlabel('brtimestamp')
    # plt.ylabel('ZDC scaled trigger rate [Hz]')
    # plt.title('brtimestamp vs. ZDC scaled trigger rate')
    # plt.xticks(rotation=45)
    # plt.legend()
    # plt.tight_layout()
    # plt.yscale('log')
    # plt.show()

if __name__ == '__main__':
    main()
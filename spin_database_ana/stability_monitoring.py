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

import ROOT


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
    ORDER BY run.runnumber;
'''%(run_year_cut, run_type_cut, run_runnumber_cut, mbd_index, run_year_cut)

spinDB_hostname = 'sphnxdbmaster.sdcc.bnl.gov'
spinDB_database = 'spinDB'
spinDB_username = 'phnxrc'
spinDB_pwd      = 'admin_test' # note : not pretty sure it is correct or not.
spinDB_port_id  = '5432'
spinDB_run_script = '''
    select runnumber,fillnumber,
        polarblue[1],polarblueerror[1],
        polaryellow[1],polaryellowerror[1],
        neutronxnorthzdc,
        neutronynorthzdc,
        neutronxsouthzdc,
        neutronysouthzdc,
        asymbfzdc,
        asymbbzdc,
        asymyfzdc,
        asymybzdc,
        asymerrbfzdc,
        asymerrbbzdc,
        asymerryfzdc,
        asymerrybzdc,
        phasebfzdc,
        phasebbzdc,
        phaseyfzdc,
        phaseybzdc,
        phaseerrbfzdc,
        phaseerrbbzdc,
        phaseerryfzdc,
        phaseerrybzdc,
        lrbluezdc,
        udbluezdc,
        lryellowzdc,
        udyellowzdc,
        lrblueerr,
        udblueerr,
        lryellowerrzdc,
        udyellowerrzdc,
        lrblueerrzdc,
        udblueerrzdc 
    from spin order by runnumber;
'''

color_code = ["#1167b1", "#c48045", "#D8364D", "#7c9885", "#28666e", "#000000"]

output_directory = "/sphenix/user/ChengWei/coldQCD/spin_database_ana/stability_plots"

def get_daq_df():
    daq_cur,daq_conn = initialize_psql_server(daq_hostname, daq_database, daq_username, daq_pwd, daq_port_id)
    run_mbdevt_df = show_cali_table(daq_cur,daq_conn, run_mbdevt_script)
    run_mbdevt_df['brtimestamp'] = pd.to_datetime(run_mbdevt_df['brtimestamp'])
    return run_mbdevt_df.fillna(-99999)


def get_spinDB_df():
    spinDB_cur,spinDB_conn = initialize_psql_server(spinDB_hostname, spinDB_database, spinDB_username, spinDB_pwd, spinDB_port_id)
    # note : grabe the table from the psql server, and make the dataframe
    spinDB_df = show_cali_table(spinDB_cur,spinDB_conn, spinDB_run_script)    
    return spinDB_df.fillna(-99999)


def combine_two_df(df_1, df_2, by_column, how = 'inner'):
    return pd.merge(df_1, df_2, on = by_column, how = how)


def prepare_good_df(polarization_cut, duration_cut):
    run_mbdevt_df = get_daq_df()
    spinDB_df = get_spinDB_df()
    combined_df = combine_two_df(run_mbdevt_df, spinDB_df, 'runnumber', how = 'inner')
    combined_df.query('polarblue[1] > %i and polaryellow[1] > %i and duration_minutes > %i'%(polarization_cut, polarization_cut, duration_cut), inplace = True)
    combined_df.query('neutronxnorthzdc != -99999.0 and neutronynorthzdc != -99999.0 and neutronxsouthzdc != -99999.0 and neutronysouthzdc != -99999.0 and asymbfzdc != -99999.0 and asymbbzdc != -99999.0 and asymyfzdc != -99999.0 and asymybzdc != -99999.0 and asymerrbfzdc != -99999.0 and asymerrbbzdc != -99999.0 and asymerryfzdc != -99999.0 and asymerrybzdc != -99999.0 and phasebfzdc != -99999.0 and phasebbzdc != -99999.0 and phaseyfzdc != -99999.0 and phaseybzdc != -99999.0 and phaseerrbfzdc != -99999.0 and phaseerrbbzdc != -99999.0 and phaseerryfzdc != -99999.0 and phaseerrybzdc != -99999.0 and lrbluezdc != -99999.0 and udbluezdc != -99999.0 and lryellowzdc != -99999.0 and udyellowzdc != -99999.0 and lrblueerr != -99999.0 and udblueerr != -99999.0 and lryellowerrzdc != -99999.0 and udyellowerrzdc != -99999.0 and lrblueerrzdc != -99999.0 and udblueerrzdc != -99999.0', 
                      inplace = True
    )

    return combined_df

def make_TGraph(df, canvas, X_axis, Y_axis, X_title, Y_title, graph_name, output_name, Y_range = [], draw_H_line = []):
    x = np.array(df[X_axis], dtype='d')
    y = np.array(df[Y_axis], dtype='d')

    n = len(x)

    if n == 0:
        print('The input axes: %s, %s. But No data for the TGraph'%(X_axis, Y_axis))
        return
    
    if len(x) != len(y):
        print('The input axes: %s, %s. But The length of x and y are different'%(X_axis, Y_axis))
        return


    graph = ROOT.TGraph(n, x, y)
    graph.SetTitle(graph_name)
    graph.SetMarkerStyle(20)
    graph.SetMarkerColor(ROOT.TColor.GetColor(color_code[0]))
    graph.GetXaxis().SetTitle(X_title) 
    graph.GetYaxis().SetTitle(Y_title)

    if len(Y_range) == 2:
        graph.GetYaxis().SetRangeUser(Y_range[0], Y_range[1])

    canvas.cd()
    graph.Draw('AP')

    draw_line = []
    if len(draw_H_line) > 0:
        for i in range(len(draw_H_line)):
            draw_line.append(ROOT.TLine(graph.GetXaxis().GetXmin(), draw_H_line[i][0], graph.GetXaxis().GetXmax(), draw_H_line[i][0]))
            draw_line[i].SetLineColor(ROOT.TColor.GetColor(color_code[i]))
            draw_line[i].SetLineStyle(7)
            draw_line[i].Draw("l same")
    
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i in range(len(draw_line)):
        legend.AddEntry(draw_line[i], '%s'%draw_H_line[i][1], 'l')

    if len(draw_H_line) > 0:
        legend.Draw("same")

    canvas.Print(output_name)
    canvas.Clear()

    return graph


def make_TGraphErrors(df, canvas, X, Y, Xe, Ye, X_title, Y_title, graph_name, output_name, Y_range = [], draw_H_line = []):
    x = np.array(df[X], dtype='d')
    y = np.array(df[Y], dtype='d')

    if Xe == 0:
        xe = np.zeros(len(x), dtype='d')
    else:
        xe = np.array(df[Xe], dtype='d')

    if Ye == 0:
        ye = np.zeros(len(y), dtype='d')
    else:
        ye = np.array(df[Ye], dtype='d')    

    n = len(x)

    if n == 0:
        print('The input axes: %s, %s, %s, %s. But No data for the TGraph'%(X, Y, Xe, Ye))
        return
    
    if len(x) != len(y):
        print('The input axes: %s, %s, %s, %s. But the N elements is different in the two axes'%(X, Y, Xe, Ye))
        return

    graph = ROOT.TGraphErrors(n, x, y, xe, ye)
    graph.SetMarkerStyle(20)
    graph.SetTitle(graph_name)
    graph.SetMarkerColor(ROOT.TColor.GetColor(color_code[0]))
    graph.GetXaxis().SetTitle(X_title) 
    graph.GetYaxis().SetTitle(Y_title)

    if len(Y_range) == 2:
        graph.GetYaxis().SetRangeUser(Y_range[0], Y_range[1])

    canvas.cd()
    graph.Draw('AP')

    draw_line = []
    if len(draw_H_line) > 0:
        for i in range(len(draw_H_line)):
            draw_line.append(ROOT.TLine(graph.GetXaxis().GetXmin(), draw_H_line[i][0], graph.GetXaxis().GetXmax(), draw_H_line[i][0]))
            draw_line[i].SetLineColor(ROOT.TColor.GetColor(color_code[i]))
            draw_line[i].SetLineStyle(7)
            draw_line[i].Draw("l same")
    
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i in range(len(draw_line)):
        legend.AddEntry(draw_line[i], '%s'%draw_H_line[i][1], 'l')

    if len(draw_H_line) > 0:
        legend.Draw("same")

    canvas.Print(output_name)
    canvas.Clear()

    return graph


def draw_multi_TGraphErrors(graphs, canvas, X_title, Y_title, output_name, draw_H_line = [], range_X = [], range_Y = []):
    canvas.cd()

    for i in range(len(graphs)):

        graphs[i].SetMarkerColor(ROOT.TColor.GetColor(color_code[i]))

        if i == 0:
            if len(range_Y) == 2:
                graphs[i].GetYaxis().SetRangeUser(range_Y[0], range_Y[1])
            if len(range_X) == 2:
                graphs[i].GetXaxis().SetLimits(range_X[0], range_X[1])
            graphs[i].GetXaxis().SetTitle(X_title)
            graphs[i].GetYaxis().SetTitle(Y_title)
            graphs[i].Draw('AP')
        else:
            graphs[i].Draw('P same')

    draw_line = []
    if len(draw_H_line) > 0:
        for i in range(len(draw_H_line)):
            draw_line.append(ROOT.TLine(graphs[0].GetXaxis().GetXmin(), draw_H_line[i][0], graphs[0].GetXaxis().GetXmax(), draw_H_line[i][0]))
            draw_line[i].SetLineColor(ROOT.TColor.GetColor(color_code[i]))
            draw_line[i].SetLineStyle(7)
            draw_line[i].Draw("l same")
    
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i in range(len(graphs)):
        legend.AddEntry(graphs[i], '%s'%graphs[i].GetTitle(), 'p')
    for i in range(len(draw_line)):
        legend.AddEntry(draw_line[i], '%s'%draw_H_line[i][1], 'l')

    legend.Draw("same")
    canvas.Print(output_name)


def make_TH1F(df, canvas, hist_setting, array_in, X_title, Y_title, output_name):
    array = np.array(df[array_in])

    hist = ROOT.TH1F('hist', ';%s;%s'%(X_title,Y_title), hist_setting[0], hist_setting[1], hist_setting[2])
    
    for value in array:
        hist.Fill(value)

    canvas.cd()
    hist.Draw("hist")
    canvas.Print(output_name)
    canvas.Clear()

    return hist


def make_TH2F(df, canvas, hist_setting, array_in_X, array_in_Y, X_title, Y_title, output_name):
    array_X = np.array(df[array_in_X])
    array_Y = np.array(df[array_in_Y])


    hist = ROOT.TH1F('hist', ';%s;%s'%(X_title,Y_title), hist_setting[0], hist_setting[1], hist_setting[2], hist_setting[3], hist_setting[4], hist_setting[5])
    
    for i in range(len(array_X)):
        hist.Fill(array_X[i], array_Y[i])

    canvas.cd()
    hist.Draw("colz0")
    canvas.Print(output_name)
    canvas.Clear()


def main():
    ROOT.gROOT.SetBatch( True )
    good_df = prepare_good_df(polarization_cut = 25, duration_cut = 10)
    print(good_df.to_string())
    print('number of good runs: %i'%good_df.shape[0])

    c1 = ROOT.TCanvas('c1', 'c1', 2000, 600)
    c1.cd()

    polarblue_gr = make_TGraphErrors(
        good_df, c1, 
        'runnumber', 'polarblue', 
        0, 'polarblueerror', 
        'Run Number', 'Polarization [%]', 
        'polarblue',
        '%s/polarblue.pdf'%output_directory,
        [0, 110],
        [[25, '25% Polarization'], [50, '50% Polarization'], [75, '75% Polarization'], [100, '100% Polarization']]
    )

    polaryellow_gr = make_TGraphErrors(
        good_df, c1, 
        'runnumber', 'polaryellow', 
        0, 'polaryellowerror', 
        'Run Number', 'Polarization [%]', 
        'polaryellow',
        '%s/polaryellow.pdf'%output_directory,
        [0, 110],
        [[25, '25% Polarization'], [50, '50% Polarization'], [75, '75% Polarization'], [100, '100% Polarization']]
    )

    phasebfzdc_gr = make_TGraphErrors(
        good_df, c1,
        'runnumber', 'phasebfzdc',
        0, 'phaseerrbfzdc',
        'Run Number', 'Phase [radian]',
        'phase blue forward zdc',
        '%s/phasebfzdc.pdf'%output_directory,
        Y_range = [-3.5, 3.5]
    )

    phasebbzdc_gr = make_TGraphErrors(
        good_df, c1,
        'runnumber', 'phasebbzdc',
        0, 'phaseerrbbzdc',
        'Run Number', 'Phase [radian]',
        'phase blue backward zdc',
        '%s/phasebbzdc.pdf'%output_directory,
        Y_range = [-3.5, 3.5]
    )

    phaseyfzdc_gr = make_TGraphErrors(
        good_df, c1,
        'runnumber', 'phaseyfzdc',
        0, 'phaseerryfzdc',
        'Run Number', 'Phase [radian]',
        'phase yellow forward zdc',
        '%s/phaseyfzdc.pdf'%output_directory,
        Y_range = [-3.5, 3.5]
    )

    phaseybzdc_gr = make_TGraphErrors(
        good_df, c1,
        'runnumber', 'phaseybzdc',
        0, 'phaseerrybzdc',
        'Run Number', 'Phase [radian]',
        'phase yellpw backward zdc',
        '%s/phaseybzdc.pdf'%output_directory,
        Y_range = [-3.5, 3.5]
    )

    neutronxnorthzdc_gr = make_TGraph(
        good_df, c1, 
        'runnumber', 'neutronxnorthzdc',  
        'Run Number', 'Mean Neutron Xpos North [cm]', 
        'neutronxnorthzdc',
        '%s/neutronxnorthzdc.pdf'%output_directory
    )

    neutronynorthzdc_gr = make_TGraph(
        good_df, c1, 
        'runnumber', 'neutronynorthzdc',  
        'Run Number', 'Mean Neutron Y North [cm]',
        'neutronynorthzdc', 
        '%s/neutronynorthzdc.pdf'%output_directory
    )

    neutronxsouthzdc_gr = make_TGraph(
        good_df, c1, 
        'runnumber', 'neutronxsouthzdc',  
        'Run Number', 'Mean Neutron X South [cm]',
        'neutronxsouthzdc', 
        '%s/neutronxsouthzdc.pdf'%output_directory
    )

    neutronysouthzdc_gr = make_TGraph(
        good_df, c1, 
        'runnumber', 'neutronysouthzdc',  
        'Run Number', 'Mean Neutron Y South [cm]',
        'neutronysouthzdc', 
        '%s/neutronysouthzdc.pdf'%output_directory
    )

    

if __name__ == '__main__':
    main()



import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import argparse

def make_plot_scatter(df, column_name_array, mark_color, Xtitle, Ytitle, PlotTitle, output_directory, output_name, draw_line = True, err_bar_axis="Y", error_bar_factor=1.):
    plt.figure(figsize=(10, 7))
    ax = plt.gca()
    if draw_line:
        plt.plot(df[column_name_array[0]], df[column_name_array[1]], linestyle='-', color = 'black', zorder=0)
    if (len(column_name_array) == 2):
        plt.scatter(df[column_name_array[0]], df[column_name_array[1]], s=65, marker = '*', color = mark_color, zorder=1)
    elif (len(column_name_array) == 3 and err_bar_axis == "Y"):
        plt.errorbar(df[column_name_array[0]], df[column_name_array[1]], yerr=df[column_name_array[2]]*error_bar_factor, fmt='o', color = mark_color, zorder=1)
        plt.text(x=0.05, y=0.9, transform=ax.transAxes, s='error bars x %i for visibility'%error_bar_factor, ha='left', va='bottom', fontsize=12)
    elif (len(column_name_array) == 3 and err_bar_axis == "X"):
        plt.errorbar(df[column_name_array[0]], df[column_name_array[1]], xerr=df[column_name_array[2]], fmt='o', color = mark_color, zorder=1)
    elif (len(column_name_array) == 4):
        plt.errorbar(df[column_name_array[0]], df[column_name_array[1]], xerr=df[column_name_array[2]], yerr=df[column_name_array[3]], fmt='o', color = mark_color, zorder=1)
    plt.xlabel(Xtitle)
    plt.ylabel(Ytitle)
    plt.title(PlotTitle)
    plt.xticks(rotation=45)
    plt.savefig("%s/%s.pdf"%(output_directory, output_name), format="pdf")
    print("the plot", output_name, "has been saved in", output_directory)

def make_multi_plot_overlap(df, Xcolumn, Ycolumn=[], mark_color=[], Xtitle='', Ytitle='', PlotTitle='', output_directory='', output_name='', legend=[], draw_line = True, Yerror=[], error_bar_factor=1.):
    plt.figure(figsize=(10, 7))
    ax = plt.gca()
    
    if (len(Yerror) != 0 and len(Ycolumn) != len(Yerror)):
        print("The length of the Yerror is not the same as the Ycolumn")
        return

    if draw_line:
        for i in range(len(Ycolumn)):
            plt.plot(df[Xcolumn], df[Ycolumn[i]], linestyle='-', color = 'black', zorder=0)

    if len(Yerror) == 0:
        for i in range(len(Ycolumn)):
            plt.scatter(df[Xcolumn], df[Ycolumn[i]], s=65, marker = '*', color = mark_color[i], zorder=1, label=legend[i])
    else:
        for i in range(len(Ycolumn)):
            plt.errorbar(df[Xcolumn], df[Ycolumn[i]], yerr=df[Yerror[i]]*error_bar_factor, fmt='o', color = mark_color[i], zorder=1, label=legend[i])
            plt.text(x=0.05, y=0.9, transform=ax.transAxes, s='error bars x %i for visibility'%error_bar_factor, ha='left', va='bottom', fontsize=12)

    plt.xlabel(Xtitle)
    plt.ylabel(Ytitle)
    plt.title(PlotTitle)
    plt.xticks(rotation=45)
    plt.legend()
    plt.savefig("%s/%s.pdf"%(output_directory, output_name), format="pdf")
    print("the plot", output_name, "has been saved in", output_directory)

    
def main():
    parser = argparse.ArgumentParser(description='Example script with command-line arguments.')
    parser.add_argument('--DrawPlotSingle', action='store_true', help='Draw the plot of single scan or not')
    parser.add_argument('--DrawPlotCombine', action='store_true', help='Draw the plot of combined or not')
    parser.add_argument('--sPHrun', type=int, help='The sPHENIX run number of the Vernier scans to be analyzed')
    parser.add_argument('--genCSV', action='store_true', help='Generate the CSV file or not')

    args = parser.parse_args()

    if (args.sPHrun == None):
        print("Please provide the sPHENIX run number of the Vernier scans to be analyzed")
        return
    elif (args.sPHrun == 48029):
        input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/CAD_reading"
        input_filenameX = "sPhenix.WcmDcctBpm.x.34785.csv"
        input_filenameY = "sPhenix.WcmDcctBpm.y.34785.csv"
        steer_beam = "Y" # note : yello beam    
        beam_color = "#FFAE42"
    elif (args.sPHrun == 51195):
        input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading"
        input_filenameX = "sPhenix.WcmDcctBpm.x.34949.dat"
        input_filenameY = "sPhenix.WcmDcctBpm.y.34949.dat"
        steer_beam = "B" # note : blue beam
        DX_SD_tag = True # note : this file has the stadard deviation of the BPMs 
        beam_color = "blue"
    else:
        print("Hey !, check what you have filled, we don't have that run for the Vernier scan!!")
        return


    output_directory = input_directory + "/plots"

    df_X = pd.read_csv(input_directory + "/" + input_filenameX, sep='\t') # note : H 
    df_Y = pd.read_csv(input_directory + "/" + input_filenameY, sep='\t') # note : V

    df_X['Average_H'] = (df_X['Dx%s7H'%steer_beam] + df_X['Dx%s8H'%steer_beam]) / 2
    df_Y['Average_V'] = (df_Y['Dx%s7V'%steer_beam] + df_Y['Dx%s8V'%steer_beam]) / 2

    if (DX_SD_tag):
        df_X['StdDev_H'] = np.sqrt( np.power(df_X['Dx%s7H_SD'%steer_beam], 2) + np.power(df_X['Dx%s8H_SD'%steer_beam], 2) ) / 2.
        df_Y['StdDev_V'] = np.sqrt( np.power(df_Y['Dx%s7V_SD'%steer_beam], 2) + np.power(df_Y['Dx%s8V_SD'%steer_beam], 2) ) / 2. 

    # df_X['DCCT_B_ratio'] = df_X['DCCT_B'] / df_X['DCCT_B'].iloc[0]
    # df_X['DCCT_Y_ratio'] = df_X['DCCT_Y'] / df_X['DCCT_Y'].iloc[0]
    # df_Y['DCCT_B_ratio'] = df_Y['DCCT_B'] / df_Y['DCCT_B'].iloc[0]
    # df_Y['DCCT_Y_ratio'] = df_Y['DCCT_Y'] / df_Y['DCCT_Y'].iloc[0]

    # # note : the systematic correction for the DCCT X scan, scale the X scan to that of the Y scan, Y scan is the first scan
    # df_X['DCCT_B_sys_scale'] = df_Y['DCCT_B'].iloc[0] / df_X['DCCT_B'].iloc[0]
    # df_X['DCCT_Y_sys_scale'] = df_Y['DCCT_Y'].iloc[0] / df_X['DCCT_Y'].iloc[0] 

    # df_X['DCCT_combined_ratio'] = 1./ (df_X['DCCT_B_ratio'] * df_X['DCCT_Y_ratio'])
    # df_Y['DCCT_combined_ratio'] = 1./ (df_Y['DCCT_B_ratio'] * df_Y['DCCT_Y_ratio'])

    if (df_Y['Time'].iloc[0] < df_X['Time'].iloc[0]):
        blue_beam_DCCT_ref = df_Y['DCCT_B'].iloc[0]
        yellow_beam_DCCT_ref = df_Y['DCCT_Y'].iloc[0]
        global_time_ref = df_Y['Time'].iloc[0]
    else:
        blue_beam_DCCT_ref = df_X['DCCT_B'].iloc[0]
        yellow_beam_DCCT_ref = df_X['DCCT_Y'].iloc[0]
        global_time_ref = df_X['Time'].iloc[0]

    # note : X scan
    df_X['relative_Time'] = df_X['Time'] - global_time_ref
    df_X['DCCT_B_ratio'] = df_X['DCCT_B'] / blue_beam_DCCT_ref   # note : blue beam
    df_X['DCCT_Y_ratio'] = df_X['DCCT_Y'] / yellow_beam_DCCT_ref # note : yellow beam
    df_X['DCCT_BY'] = df_X['DCCT_B'] * df_X['DCCT_Y']
    df_X['DCCT_correction'] = 1./ (df_X['DCCT_B_ratio'] * df_X['DCCT_Y_ratio'])
    df_X['DCCT_post_corr'] = df_X['DCCT_BY'] * df_X['DCCT_correction']
    df_X = df_X.drop(df_X.index[-1])
    
    # note : Y scan
    df_Y['relative_Time'] = df_Y['Time'] - global_time_ref
    df_Y['DCCT_B_ratio'] = df_Y['DCCT_B'] / blue_beam_DCCT_ref   # note : blue beam
    df_Y['DCCT_Y_ratio'] = df_Y['DCCT_Y'] / yellow_beam_DCCT_ref # note : yellow beam
    df_Y['DCCT_BY'] = df_Y['DCCT_B'] * df_Y['DCCT_Y']
    df_Y['DCCT_correction'] = 1./ (df_Y['DCCT_B_ratio'] * df_Y['DCCT_Y_ratio'])
    df_Y['DCCT_post_corr'] = df_Y['DCCT_BY'] * df_Y['DCCT_correction']
    df_Y = df_Y.drop(df_Y.index[-1])


    if (df_Y['Time'].iloc[0] < df_X['Time'].iloc[0]):
        df_YX = pd.concat([df_Y, df_X], axis=0, ignore_index=True) # note : do the Y scan first
    else:
        df_YX = pd.concat([df_X, df_Y], axis=0, ignore_index=True) # note : do the X scan first


    # df_YX['DCCT_B_ratio'] = df_YX['DCCT_B'] / df_YX['DCCT_B'].iloc[0] # note : blue beam
    # df_YX['DCCT_Y_ratio'] = df_YX['DCCT_Y'] / df_YX['DCCT_Y'].iloc[0] # note : yellow beam
    # df_YX['DCCT_BY'] = df_YX['DCCT_B'] * df_YX['DCCT_Y']
    # df_YX['DCCT_combined_ratio'] = 1./ (df_YX['DCCT_B_ratio'] * df_YX['DCCT_Y_ratio'])
    # df_YX['DCCT_BY_corr'] = df_YX['DCCT_BY'] * df_YX['DCCT_combined_ratio']

    pd.set_option('display.precision', 10)


    print(df_Y.to_string())
    print(df_X.to_string())
    print(df_YX.to_string())

    # first_time_X = df_X['Time'].iloc[0]
    # first_time_Y = df_Y['Time'].iloc[0]
    # first_time_YX = df_YX['Time'].iloc[0]

    # df_X['Time'] = df_X['Time'] - first_time_X
    # df_Y['Time'] = df_Y['Time'] - first_time_Y
    # df_YX['Time'] = df_YX['Time'] - first_time_YX


    print("------------------------------------------------------")
    print("the average and stddev of DCCT_B : %.4f, %.4f"%(df_YX['DCCT_B'].mean(), df_YX['DCCT_B'].std()))
    print("the average and stddev of DCCT_Y : %.4f, %.4f"%(df_YX['DCCT_Y'].mean(), df_YX['DCCT_Y'].std()))
    print("the average and stddev of WcmB : %.4f, %.4f"%(df_YX['WcmB'].mean(), df_YX['WcmB'].std()))
    print("the average and stddev of WcmY : %.4f, %.4f"%(df_YX['WcmY'].mean(), df_YX['WcmY'].std()))

    if (args.genCSV):
        sub_df_X = df_X[['Average_H','DCCT_correction']]
        sub_df_Y = df_Y[['Average_V','DCCT_correction']]

        DCCT_number_df = pd.DataFrame({
            'DCCT_B_avg': [df_YX['DCCT_B'].mean()] * len(sub_df_X), 
            'DCCT_B_std': [df_YX['DCCT_B'].std()] * len(sub_df_X), 
            'DCCT_Y_avg': [df_YX['DCCT_Y'].mean()] * len(sub_df_X), 
            'DCCT_Y_std': [df_YX['DCCT_Y'].std()] * len(sub_df_X)
        })

        if (DX_SD_tag):
            sub_df_X_SD = df_X[['StdDev_H']]
            sub_df_Y_SD = df_Y[['StdDev_V']]
            final_df = pd.concat([sub_df_X, sub_df_Y, DCCT_number_df, sub_df_X_SD, sub_df_Y_SD], axis=1)  
            print(final_df.to_string())
        else:
            final_df = pd.concat([sub_df_X, sub_df_Y, DCCT_number_df], axis=1)  
            print(final_df.to_string())

        final_df.to_csv(output_directory + "/CAD_reading_%i.csv"%(args.sPHrun), sep='\t', index=False)

    if (args.DrawPlotSingle):
        make_plot_scatter(df_X, ['relative_Time', 'WcmB'], 'blue',      'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current blue vs. time',   output_directory, 'X_Time_WcmB')
        make_plot_scatter(df_X, ['relative_Time', 'WcmY'], '#FFAE42',   'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current yellow vs. time', output_directory, 'X_Time_WcmY')

        make_plot_scatter(df_X, ['relative_Time', 'DCCT_B'], 'blue',    'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current blue vs. time',   output_directory, 'X_Time_DCCT_B')
        make_plot_scatter(df_X, ['relative_Time', 'DCCT_Y'], '#FFAE42', 'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current yellow vs. time', output_directory, 'X_Time_DCCT_Y')

        Dx_7H_array = ['relative_Time', 'Dx%s7H'%steer_beam]
        if (DX_SD_tag):
            Dx_7H_array.append('Dx%s7H_SD'%steer_beam)
        make_plot_scatter(df_X, Dx_7H_array, beam_color,  'Time in unix [$\Delta$s]', "BPM position south (horizontal) [mm]", "BPM position south (7 o'clock) vs. time",   output_directory, 'X_Time_Dx%s7H'%steer_beam, error_bar_factor=30.)
        Dx_8H_array = ['relative_Time', 'Dx%s8H'%steer_beam]
        if (DX_SD_tag):
            Dx_8H_array.append('Dx%s8H_SD'%steer_beam)
        make_plot_scatter(df_X, Dx_8H_array, beam_color,  'Time in unix [$\Delta$s]', 'BPM position north (horizontal) [mm]', "BPM position north (8 o'clock) vs. time",   output_directory, 'X_Time_Dx%s8H'%steer_beam, error_bar_factor=30.)



        make_plot_scatter(df_Y, ['relative_Time', 'WcmB'], 'blue',      'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current blue vs. time',   output_directory, 'Y_Time_WcmB')
        make_plot_scatter(df_Y, ['relative_Time', 'WcmY'], '#FFAE42',   'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current yellow vs. time', output_directory, 'Y_Time_WcmY')

        make_plot_scatter(df_Y, ['relative_Time', 'DCCT_B'], 'blue',    'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current blue vs. time',   output_directory, 'Y_Time_DCCT_B')
        make_plot_scatter(df_Y, ['relative_Time', 'DCCT_Y'], '#FFAE42', 'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current yellow vs. time', output_directory, 'Y_Time_DCCT_Y')

        Dx_7V_array = ['relative_Time', 'Dx%s7V'%steer_beam]
        if (DX_SD_tag):
            Dx_7V_array.append('Dx%s7V_SD'%steer_beam)
        make_plot_scatter(df_Y, Dx_7V_array, beam_color,  'Time in unix [$\Delta$s]', 'BPM position south (vertical) [mm]', "BPM position south (7 o'clock) vs. time",   output_directory, 'Y_Time_Dx%s7V'%steer_beam, error_bar_factor=30.)
        Dx_8V_array = ['relative_Time', 'Dx%s8V'%steer_beam]
        if (DX_SD_tag):
            Dx_8V_array.append('Dx%s8V_SD'%steer_beam)
        make_plot_scatter(df_Y, Dx_8V_array, beam_color,  'Time in unix [$\Delta$s]', 'BPM position north (vertical) [mm]', "BPM position north (8 o'clock) vs. time",   output_directory, 'Y_Time_Dx%s8V'%steer_beam, error_bar_factor=30.)

        Dx_H_err_bar = []
        Dx_V_err_bar = []
        if (DX_SD_tag):
            Dx_H_err_bar=['Dx%s7H_SD'%steer_beam, 'Dx%s8H_SD'%steer_beam, 'StdDev_H']
            Dx_V_err_bar=['Dx%s7V_SD'%steer_beam, 'Dx%s8V_SD'%steer_beam, 'StdDev_V']
        make_multi_plot_overlap(df_X, 'relative_Time', ['Dx%s7H'%steer_beam, 'Dx%s8H'%steer_beam, 'Average_H'], ['red', 'green', 'black'], 'Time in unix [$\Delta$s]', 'BPM position (horizontal) [mm]', 'BPM position vs. time', output_directory, 'X_Time_Dx%s7H_Dx%s8H'%(steer_beam, steer_beam), ["BPM position south (7 o'clock)", "BPM position north (8 o'clock)", "Average"], Yerror=Dx_H_err_bar, error_bar_factor=30.)
        make_multi_plot_overlap(df_Y, 'relative_Time', ['Dx%s7V'%steer_beam, 'Dx%s8V'%steer_beam, 'Average_V'], ['red', 'green', 'black'], 'Time in unix [$\Delta$s]', 'BPM position (vertical) [mm]', 'BPM position vs. time', output_directory, 'Y_Time_Dx%s7V_Dx%s8V'%(steer_beam, steer_beam), ["BPM position south (7 o'clock)", "BPM position north (8 o'clock)", "Average"], Yerror=Dx_V_err_bar, error_bar_factor=30.)

    if (args.DrawPlotCombine):
        # note : the followings are for the Y and X combined
        make_plot_scatter(df_YX, ['relative_Time', 'WcmB'], 'blue',      'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current blue vs. time',   output_directory, 'YX_Time_WcmB')
        make_plot_scatter(df_YX, ['relative_Time', 'WcmY'], '#FFAE42',   'Time in unix [$\Delta$s]', 'Wall Current [10^9]', 'Wall current yellow vs. time', output_directory, 'YX_Time_WcmY')

        make_plot_scatter(df_YX, ['relative_Time', 'DCCT_B'], 'blue',    'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current blue vs. time',   output_directory, 'YX_Time_DCCT_B')
        make_plot_scatter(df_YX, ['relative_Time', 'DCCT_Y'], '#FFAE42', 'Time in unix [$\Delta$s]', 'Direct Current [10^9]', 'Direct Current yellow vs. time', output_directory, 'YX_Time_DCCT_Y')
        

    
        

if __name__ == '__main__':
    main()
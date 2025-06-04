import numpy as np
from datetime import datetime
import json
import sys
import os

def process_file(filename):
    columns = [
        "Sim_Name", "Begin_of_run", "End_of_run", "SHMS_angle", 
        "Energy", "HMS_P", "HMS_angle", "NPS_angle", "Calo_Distance", "HMS_pol"
    ]
    data = {col: [] for col in columns}

    with open(filename, 'r') as file:
        next(file)
        current_simulation_name = None
        
        for line in file:
            line = line.strip()
            
            if not line:
                continue
            
            parts = line.split("\t")
            
            if not parts[0].isdigit():
                current_simulation_name = parts[0]
                data["Sim_Name"].append(current_simulation_name)
                data["Begin_of_run"].append(parts[1])
                data["End_of_run"].append(parts[2])
                for i, col in enumerate(columns[3:], start=3):
                    data[col].append(parts[i])


    for col in columns:
        data[col] = np.array(data[col])

    return data




def generate_output_files(data, output_dir):
    # for col, values in data.items():
    #     print(f"{col}: {values}")
    
    # print(len(data["Sim_Name"]))

    date_str = datetime.now().strftime("%m/%d/%Y")

    num_entries = len(data["Sim_Name"]) 
    for idx in range(num_entries):

        Simulation_name = data["Sim_Name"][idx]
        hms_momentum = float(data["HMS_P"][idx]) * 1000
        hms_angle = float(data["HMS_angle"][idx])

        hms_offset_H = (0.52-0.012*hms_angle+0.002*hms_angle*hms_angle)*0.1
        hms_offset_V = (2.37-0.086*hms_angle+0.0012*hms_angle*hms_angle)*0.1

        hms_pol = float(data["HMS_pol"][idx])
        
        Run_start = data["Begin_of_run"][idx]
        Run_end = data["End_of_run"][idx]
        shms_angle = float(data["SHMS_angle"][idx])
        Beam_energy = float(data["Energy"][idx])
        nps_angle = float(data["NPS_angle"][idx])
        calo_distance = float(data["Calo_Distance"][idx])

        output_content_LH2 = f"""\
!------------------------------------------------------------------------------
! Input file for mc_single_arm 					{date_str}
! NPS version - using pre-generated events
!------------------------------------------------------------------------------
        1       Spectrometer (1=HMS, 2=SHMS, 3=..)
     {hms_momentum:.1f}	Spectrometer momentum (in MeV/c)
     {hms_angle:.3f}	Spectrometer angle (in degrees)
     0.0        Horiz beam spot size in cm (Full width of +/- 3 sigma)
     0.0        Vert  beam spot size in cm (Full width of +/- 3 sigma)
     10.0	Thickness of target (Full width, cm)
     0.0        Raster full-width x (cm)
     0.0        Raster full-width y (cm)
     890.5	one radiation length of target material (in cm)
     0.0        Beam x offset (cm)  +x = beam left
     0.0	Beam y offset (cm)  +y = up
     0.0        Target z offset (cm)+z = downstream (0.25)
     {hms_offset_V:.4f}        Spectrometer x offset (cm) +x = down
     {hms_offset_H:.4f}        Spectrometer y offset (cm)
     0.0        Spectrometer z offset (cm)
     0.0        Spectrometer xp offset (mr)
     0.0        Spectrometer yp offset (mr)
     0		particle identification :e=0, p=1, d=2, pi=3, ka=4
     1		flag for multiple scattering
     1		flag for wire chamber smearing
     1	        flag for storing all events (including failed events with stop_id > 0)
"""

        output_content_LD2 = f"""\
!------------------------------------------------------------------------------
! Input file for mc_single_arm 					{date_str}
! NPS version - using pre-generated events
!------------------------------------------------------------------------------
        1       Spectrometer (1=HMS, 2=SHMS, 3=..)
     {hms_momentum:.1f}	Spectrometer momentum (in MeV/c)
     {hms_angle:.3f}	Spectrometer angle (in degrees)
     0.0        Horiz beam spot size in cm (Full width of +/- 3 sigma)
     0.0        Vert  beam spot size in cm (Full width of +/- 3 sigma)
     10.0	Thickness of target (Full width, cm)
     0.0        Raster full-width x (cm)
     0.0        Raster full-width y (cm)
     769.1	one radiation length of target material (in cm)
     0.0        Beam x offset (cm)  +x = beam left
     0.0	Beam y offset (cm)  +y = up
     0.0        Target z offset (cm)+z = downstream (0.25)
     {hms_offset_V:.4f}        Spectrometer x offset (cm) +x = down
     {hms_offset_H:.4f}        Spectrometer y offset (cm)
     0.0        Spectrometer z offset (cm)
     0.0        Spectrometer xp offset (mr)
     0.0        Spectrometer yp offset (mr)
     0		particle identification :e=0, p=1, d=2, pi=3, ka=4
     1		flag for multiple scattering
     1		flag for wire chamber smearing
     1	        flag for storing all events (including failed events with stop_id > 0)
"""

        output_filename_LH2 = f"{output_dir}/hms_LH2_{Simulation_name}.inp"
        output_filename_LD2 = f"{output_dir}/hms_LD2_{Simulation_name}.inp"
        
        with open(output_filename_LH2, "w") as file:
            file.write(output_content_LH2)

        with open(output_filename_LD2, "w") as file:
            file.write(output_content_LD2)

        print(f"Generated file: {output_filename_LH2}")
        print(f"Generated file: {output_filename_LD2}")



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 Generate_Input_Files.py KinC_list_*.txt output_dir")
        sys.exit(1)

    KinC_list = sys.argv[1]
    output_directory = sys.argv[2]
    os.makedirs(output_directory, exist_ok=True)

    data = process_file(KinC_list)
    generate_output_files(data, output_directory)

using CSV
using DataFrames
using FFTW
using Plots
using LaTeXStrings

theme(:bright)
plot_font = "Computer Modern"
default(fontfamily=plot_font,linewidth=2, framestyle=:box, label=nothing, grid=false)
scalefontsizes(1.3)

FilePath = "NewMotorNominalRun.csv";
Data = DataFrame(CSV.File(FilePath,header = 1, delim = ","));


names(Data)
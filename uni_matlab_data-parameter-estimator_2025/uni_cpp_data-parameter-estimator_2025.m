clear;
clc;


file_front_gate = 'dataset_front_gate.csv';
file_nassau_gate = 'dataset_nassau_st_gate.csv';
file_pearse_gate = 'dataset_pearse_st_gate.csv';

fileList = {file_front_gate, file_nassau_gate, file_pearse_gate};
locationNames = {'Front Gate', 'Nassau St Gate', 'Pearse St Gate'};

for i = 1:length(fileList)

    currentFile = fileList{i};
    currentLocation = locationNames{i};

    fprintf('--- Gate: %s ---\n', currentLocation);

    dataTable = readtable(currentFile);
    soundData = dataTable.sound_data_dB;

    % 1. Log-normal
    try
        pd_logn = fitdist(soundData, 'Lognormal');
        mu_logn = pd_logn.mu;       % mu (mean of log(data))
        sigma_logn = pd_logn.sigma; % sigma (std dev of log(data))
        fprintf('  Log-normal Parameters: mu = %.4f, sigma = %.4f\n', mu_logn, sigma_logn);
    catch ME_logn
        fprintf('  Log-normal Fit Error: %s\n', ME_logn.message);
    end

    % 2. Weibull
    try
        pd_wbl = fitdist(soundData, 'Weibull');
        A_wbl = pd_wbl.A; % lambda (scale)
        B_wbl = pd_wbl.B; % k (shape)
        fprintf('  Weibull Parameters: A (scale) = %.4f, B (shape) = %.4f\n', A_wbl, B_wbl);
    catch ME_wbl
        fprintf('  Weibull Fit Error: %s\n', ME_wbl.message);
    end

    % 3. Exponential
    try
        pd_exp = fitdist(soundData, 'Exponential');
        mu_exp = pd_exp.mu; % mu (mean)
        fprintf('  Exponential Parameter: mu (mean) = %.4f\n', mu_exp);
    catch ME_exp
        fprintf('  Exponential Fit Error: %s\n', ME_exp.message);
    end

end

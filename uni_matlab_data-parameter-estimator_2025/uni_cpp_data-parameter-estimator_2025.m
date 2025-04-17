clear;
clc;

% Load datasets
file_front_gate = 'dataset_front_gate.csv';
file_nassau_gate = 'dataset_nassau_st_gate.csv';
file_pearse_gate = 'dataset_pearse_st_gate.csv';
fileList = {file_front_gate, file_nassau_gate, file_pearse_gate};
locationNames = {'Front Gate', 'Nassau St Gate', 'Pearse St Gate'};

% Initialize a cell array to store results
results = cell(9, 6); % 9 rows (3 locations × 3 distributions), 6 columns
headers = {'Location', 'Distribution', 'Parameters', 'LogLikelihood', 'AIC', 'BIC'};
for i = 1:6
    results{1, i} = headers{i};
end
row = 2;

for i = 1:length(fileList)
    currentFile = fileList{i};
    currentLocation = locationNames{i};
    fprintf('\n--- Gate: %s ---\n', currentLocation);
    
    % Read data
    dataTable = readtable(currentFile);
    % Extract sound data column - adjust the column name to match your data
    soundData = dataTable.sound_data_dB_;
    
    % 1. Log-normal
    try
        pd_logn = fitdist(soundData, 'Lognormal');
        mu_logn = pd_logn.mu;       % mu (mean of log(data))
        sigma_logn = pd_logn.sigma; % sigma (std dev of log(data))
        fprintf('  Log-normal Parameters: mu = %.4f, sigma = %.4f\n', mu_logn, sigma_logn);
        
        % Calculate log-likelihood manually
        logL_logn = sum(log(pdf(pd_logn, soundData)));
        n = length(soundData);
        aic_logn = -2*logL_logn + 2*2;  % 2 parameters
        bic_logn = -2*logL_logn + 2*log(n);
        
        % Store in results table
        results{row, 1} = currentLocation;
        results{row, 2} = 'Log-normal';
        results{row, 3} = sprintf('mu=%.4f, sigma=%.4f', mu_logn, sigma_logn);
        results{row, 4} = logL_logn;
        results{row, 5} = aic_logn;
        results{row, 6} = bic_logn;
        row = row + 1;
    catch ME_logn
        fprintf('  Log-normal Fit Error: %s\n', ME_logn.message);
    end
    
    % 2. Weibull
    try
        pd_wbl = fitdist(soundData, 'Weibull');
        A_wbl = pd_wbl.A; % lambda (scale)
        B_wbl = pd_wbl.B; % k (shape)
        fprintf('  Weibull Parameters: A (scale) = %.4f, B (shape) = %.4f\n', A_wbl, B_wbl);
        
        % Calculate log-likelihood manually
        logL_wbl = sum(log(pdf(pd_wbl, soundData)));
        aic_wbl = -2*logL_wbl + 2*2;  % 2 parameters
        bic_wbl = -2*logL_wbl + 2*log(n);
        
        % Store in results
        results{row, 1} = currentLocation;
        results{row, 2} = 'Weibull';
        results{row, 3} = sprintf('A=%.4f, B=%.4f', A_wbl, B_wbl);
        results{row, 4} = logL_wbl;
        results{row, 5} = aic_wbl;
        results{row, 6} = bic_wbl;
        row = row + 1;
    catch ME_wbl
        fprintf('  Weibull Fit Error: %s\n', ME_wbl.message);
    end
    
    % 3. Exponential
    try
        pd_exp = fitdist(soundData, 'Exponential');
        mu_exp = pd_exp.mu; % mu (mean)
        fprintf('  Exponential Parameter: mu (mean) = %.4f\n', mu_exp);
        
        % Calculate log-likelihood manually
        logL_exp = sum(log(pdf(pd_exp, soundData)));
        aic_exp = -2*logL_exp + 2*1;  % 1 parameter
        bic_exp = -2*logL_exp + 1*log(n);
        
        % Store in results
        results{row, 1} = currentLocation;
        results{row, 2} = 'Exponential';
        results{row, 3} = sprintf('mu=%.4f', mu_exp);
        results{row, 4} = logL_exp;
        results{row, 5} = aic_exp;
        results{row, 6} = bic_exp;
        row = row + 1;
    catch ME_exp
        fprintf('  Exponential Fit Error: %s\n', ME_exp.message);
    end
    
    % Perform Chi-square goodness-of-fit test
    fprintf('\n  Chi-square Goodness-of-fit Tests:\n');
    try
        [h_logn, p_logn] = chi2gof(soundData, 'CDF', pd_logn);
        fprintf('  Log-normal: p-value = %.4f, Reject null? %s\n', p_logn, string(h_logn));
    catch ME
        fprintf('  Log-normal Chi2 test error: %s\n', ME.message);
    end
    
    try
        [h_wbl, p_wbl] = chi2gof(soundData, 'CDF', pd_wbl);
        fprintf('  Weibull: p-value = %.4f, Reject null? %s\n', p_wbl, string(h_wbl));
    catch ME
        fprintf('  Weibull Chi2 test error: %s\n', ME.message);
    end
    
    try
        [h_exp, p_exp] = chi2gof(soundData, 'CDF', pd_exp);
        fprintf('  Exponential: p-value = %.4f, Reject null? %s\n', p_exp, string(h_exp));
    catch ME
        fprintf('  Exponential Chi2 test error: %s\n', ME.message);
    end
end

% Convert cell array to table for display
resultsTable = cell2table(results(2:end,:), 'VariableNames', headers);
disp('Complete Goodness-of-fit Results:');
disp(resultsTable);

% Find best distribution for each location
fprintf('\nBest Distributions by Location:\n');
for i = 1:length(locationNames)
    loc = locationNames{i};
    fprintf('  %s:\n', loc);
    
    % Find rows for this location
    loc_rows = [];
    for j = 2:size(results,1)
        if strcmp(results{j,1}, loc)
            loc_rows = [loc_rows; j];
        end
    end
    
    % Get metrics for this location
    loc_logL = cell2mat(results(loc_rows, 4));
    loc_aic = cell2mat(results(loc_rows, 5));
    loc_bic = cell2mat(results(loc_rows, 6));
    
    % Find best distribution
    [best_logL, best_logL_idx] = max(loc_logL);
    [best_aic, best_aic_idx] = min(loc_aic);
    [best_bic, best_bic_idx] = min(loc_bic);
    
    fprintf('    Best by Log-Likelihood: %s (%.2f)\n', results{loc_rows(best_logL_idx), 2}, best_logL);
    fprintf('    Best by AIC: %s (%.2f)\n', results{loc_rows(best_aic_idx), 2}, best_aic);
    fprintf('    Best by BIC: %s (%.2f)\n', results{loc_rows(best_bic_idx), 2}, best_bic);
end

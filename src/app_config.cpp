#include "app_config.hpp"

void AppConfig::configure(CLI::App &app)
{
    app.add_option("-i, --input", input, "Path where the input file is placed.")->required()->check(CLI::ExistingFile)->ignore_case();

    app.add_option("-o, --output", output, "Path to save the result of the operation.")->required()->check(CLI::ExistingPath)->ignore_case();

    app.add_option("--op, --operation", operation, "Kind of operation (encrypt or decrypt))")->required();

    app.add_option("--itr, --iterations", iterations, "Number of iterations.")->expected(0, 2000000)->default_val(500000);

    app.add_option("-v, --verbose", verbose, "Show more information on the terminal")->default_val(false);

    app.add_option("--oof, --overwriteOriginalFile", overwrite, "Overwrite the original file to safely erase the original data from device.")->default_val(false);
    
}
#include "app_config.hpp"

void AppConfig::configure(CLI::App &app)
{
    std::string op_str;

    app.add_option("-i, --input", input, "Path where the input file is placed.")->required()->check(CLI::ExistingFile)->ignore_case();

    app.add_option("-o, --output", output, "Path to save the result of the operation.")->required()->ignore_case();

    app.add_option("--op, --operation", op_str, "Kind of operation (encrypt or decrypt))")->required();
    app.callback([&](){
        std::string op_prefix = op_str.substr(0, 3);
        std::transform(op_prefix.begin(), op_prefix.end(), op_prefix.begin(),
               [](unsigned char c) { return std::tolower(c); });

        if(op_prefix.compare(0,3, "enc") == 0)
        {
            operation = Operation::Encrypt;
        } 
        else if(op_prefix.compare(0, 3, "dec") == 0)
        {
            operation = Operation::Decrypt;
        }
        else {
            throw CLI::ValidationError("Invalid operation: must start with 'enc' or 'dec'");
        }
    });
    app.add_option("--itr, --iterations", iterations, "Number of iterations.")->expected(0, 2000000)->default_val(500000);

    app.add_option("-v, --verbose", verbose, "Show more information on the terminal")->default_val(false);

    app.add_flag("--dof, --deleteOriginalFile", deleteOriginalFile,
        "Overwrite the original file to safely erase the original data from device.")->default_val(false);
        
    app.add_option("--dof-times", deleteOverwriteTimes,
        "Number of times to overwrite the original file if deletion is enabled.")
        ->default_val(3)
        ->check(CLI::Range(1, 100))
        ->needs("--dof"); 
}
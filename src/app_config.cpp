#include "app_config.hpp"

void AppConfig::configure(CLI::App &app)
{
    std::string op_str;

    const std::map<std::string, Operation> operation_map{
        {"encrypt", Operation::Encrypt},
        {"enc", Operation::Encrypt},
        {"decrypt", Operation::Decrypt},
        {"dec", Operation::Decrypt},
    };

    app.add_option("-i, --input", input, "Path where the input file is placed.")->required()->check(CLI::ExistingFile)->ignore_case();

    app.add_option("-o, --output", output, "Path to save the result of the operation.")->required()->ignore_case();

    app.add_option("--op, --operation", operation, "Kind of operation (encrypt or decrypt)")->required()->transform(CLI::CheckedTransformer(operation_map, CLI::ignore_case));    

    app.add_option("--sl, --security-level", security_level_int, "Security level (1=Low, 2=Moderate, 3=Sensitive)")->check(CLI::Range(1, 3))->default_val(2);

    app.add_flag("-v, --verbose", verbose, "Show more information on the terminal")->ignore_case();

    app.add_flag("--dof, --deleteOriginalFile", deleteOriginalFile,
        "Overwrite the original file to safely erase the original data from device.");
        
    app.add_option("--doft, --dof-times", deleteOverwriteTimes,
        "Number of times to overwrite the original file if deletion is enabled.")
        ->default_val(3)
        ->check(CLI::Range(1, 100))
        ->needs("--dof"); 

}
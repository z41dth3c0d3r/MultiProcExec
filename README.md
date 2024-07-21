# MultiProcExec

MultiProcExec is a powerful command-line utility designed to streamline the process of launching multiple applications or services simultaneously. It's particularly useful for developers working on complex projects that require running several processes concurrently, such as full-stack web applications or microservices architectures.

## Use Case

Imagine you're developing a Laravel project with a Vue.js frontend. Typically, you'd need to open multiple terminal windows to run your backend server, frontend development server, and perhaps additional services like a database or a queue worker. MultiProcExec simplifies this workflow by allowing you to define all these commands in a single configuration file and launch them with one command.

For instance, instead of manually running:

1. `php artisan serve` for your Laravel backend
2. `npm run dev` for your Vue.js frontend
3. `redis-server` for your caching service

You can create a config file with these commands and use MultiProcExec to launch all of them in separate processes with a single command.

## Features

- Execute multiple commands concurrently, each in its own process
- Use direct paths or predefined configurations
- List commands without execution for verification
- Simple, text-based configuration file format

## Installation

1.  Clone the repository:

        git clone https://github.com/z41dth3c0d3r/MultiProcExec

2.  open the .sln file in visual studio

## Usage

MultiProcExec supports the following command-line arguments:

- `-p <path>`: Specify the direct path to a config file
- `-c <name>`: Use a predefined config file by name
- `-s`: List the commands in the config file without executing them

### Using a direct path to config file

/MultiProcExec -p /path/to/your/config.txt

### Using a predefined config

1. Set the `PROC_LAUNCHER_CONFIGS` environment variable:

export PROC_LAUNCHER_CONFIGS=/path/to/your/config/directory

2. Run the program with the config name:

./MultiProcExec -c my_project_config

### Listing commands without execution

./MultiProcExec -p /path/to/your/config.txt -s

## Configuration File Format

Create a text file with one command per line. For example:

    npm run dev
    php artisan serve
    redis-server

## Example

For a Laravel project, your config file `laravel_dev.txt` might look like this:

    npm run dev
    php artisan serve

You could then run:

./MultiProcExec -p /path/to/laravel_dev.txt

Or, with the `PROC_LAUNCHER_CONFIGS` environment variable set:

./MultiProcExec -c laravel_dev

## Notes

- Ensure commands in your config file are executable from the current directory.
- The program runs until manually stopped (e.g., with Ctrl+C).
- Be aware of potential conflicts (e.g., port conflicts) between concurrent services.

## Requirements

- C++17 or later
- Currently Windows Only

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

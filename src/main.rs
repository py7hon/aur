use std::env;
use std::process;

const RED: &str = "\x1B[1m\x1B[31m";
const WHITE: &str = "\x1B[1m\x1B[37m";
const RESET: &str = "\x1B[0m";

fn print_error_message(format: &str, _arg0: &str) {
    println!("{}error:{} {}", RED, RESET, format);
    process::exit(libc::EXIT_FAILURE);
}

fn print_help_message(arg0: &str) {
    println!("usage:  {} <operation> [...]", arg0);
    println!("operations:");
    println!("    {} {{-h}}", arg0);
    println!("    {} {{-R}}  <package(s)>", arg0);
    println!("    {} {{-S}}  [package(s)]", arg0);
}

fn search_and_install_packages(args: Vec<String>) {
    if args.len() < 3 {
        print_error_message("No packages specified! Use {} -S <packages>!\n", &args[0]);
    }

    for arg in &args[2..] {
        let result = format!(
            "cd /tmp && rm -rf {} && git clone https://aur.archlinux.org/{}.git && cd {} && makepkg -si",
            arg, arg, arg
        );

        println!("{}Search Packages....{}", WHITE, RESET);
        std::thread::sleep(std::time::Duration::from_secs(5));
        let output = process::Command::new("sh")
            .arg("-c")
            .arg(&result)
            .output()
            .expect("Failed to execute command");
        println!("{}", String::from_utf8_lossy(&output.stdout));
    }
}

fn remove_packages(args: Vec<String>) {
    if args.len() < 3 {
        print_error_message("No packages specified! Use {} -R <packages>!\n", &args[0]);
    }

    let names = args[2..].join(" ");
    let result = format!("sudo pacman -R {}", names);

    let output = process::Command::new("sh")
        .arg("-c")
        .arg(&result)
        .output()
        .expect("Failed to execute command");
    println!("{}", String::from_utf8_lossy(&output.stdout));
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let rootkh = unsafe { libc::getuid() };

    if rootkh == 0 {
        print_error_message("Running {} as root is not allowed as it can cause permanent, catastrophic damage to your system.\n", &args[0]);
    }

    if args.len() < 2 {
        print_error_message("No operation specified (use -h for help)\n", &args[0]);
    }

    match args[1].as_str() {
        "-h" => print_help_message(&args[0]),
        "-S" => search_and_install_packages(args),
        "-R" => remove_packages(args),
        _ => print_error_message("Command not found!\n", &args[0]),
    }

    process::exit(libc::EXIT_SUCCESS);
}

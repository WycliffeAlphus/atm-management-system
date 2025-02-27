# ATM Application

## Installation and Setup

Follow these steps to set up and run the ATM application on your system.

### 1. Clone the Repository
```sh
git clone https://github.com/WycliffeAlphus/atm-management-system
```

### 2. Navigate to the Project Directory
```sh
cd atm-management-system
```

### 3. Set Up the Database
Run the provided script to initialize the SQLite database:
```sh
./setup_sqlite.sh
```

### 4. Build the Application
Use `make` to compile the source code:
```sh
make
```
This will generate the binary file for the ATM application.

### 5. Run the Application
Once the build is complete, run the ATM program using:
```sh
./atm
```

## Usage
- Follow the on-screen instructions to interact with the ATM system.
- Ensure the database is properly initialized before running the application.

## Notes
- If you encounter permission issues, you may need to grant execution rights to the setup script:
  ```sh
  chmod +x setup_sqlite.sh
  ```
- Run `make clean` to remove compiled files if needed.

## Troubleshooting
If you face issues, try the following:
- Ensure you are in the correct directory.
- Verify that `setup_sqlite.sh` ran successfully.
- Re-run `make` if the binary was not created.
- Check for missing dependencies.

## License
This project is licensed under [MIT License](LICENSE).


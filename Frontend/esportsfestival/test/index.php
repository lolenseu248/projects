<?php
// Database connection
$servername = "localhost";
$username = "root";
$password = ""; // Default XAMPP password is empty
$dbname = "esportsdb";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Handle form submission
$message = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $input_username = $_POST['username'] ?? '';
    $input_password = $_POST['password'] ?? '';

    // Sanitize and validate inputs
    $input_username = $conn->real_escape_string($input_username);
    $hashed_password = password_hash($input_password, PASSWORD_DEFAULT); // Hash the password

    // Insert user into database
    $sql = "INSERT INTO test_table (username, password) VALUES ('$input_username', '$hashed_password')";

    if ($conn->query($sql) === TRUE) {
        $message = "Registration successful! Welcome, " . htmlspecialchars($input_username) . ".";
    } else {
        if ($conn->errno == 1062) {
            $message = "Username already exists. Please choose another.";
        } else {
            $message = "Error: " . $conn->error;
        }
    }
}

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Register</title>
</head>
<body>
    <h2>Register</h2>
    <form action="index.php" method="post">
        <label for="username">Username:</label>
        <input type="text" name="username" id="username" required><br><br>

        <label for="password">Password:</label>
        <input type="password" name="password" id="password" required><br><br>

        <button type="submit">Register</button>
    </form>
    <?php if ($message): ?>
        <p style="color: green;"><?php echo htmlspecialchars($message); ?></p>
    <?php endif; ?>
</body>
</html>

<?php
// Database connection
include('connection.php');

// Initialize variables
$searchQuery = '';
$results = [];

// Process search if the form is submitted
if (isset($_GET['search'])) {
    $searchQuery = $conn->real_escape_string($_GET['search']);

    // Query the database
    $sql = "SELECT id, username, role 
            FROM users_data 
            WHERE username LIKE '%$searchQuery%' OR role LIKE '%$searchQuery%'
            ORDER BY username ASC";

    $result = $conn->query($sql);

    if ($result && $result->num_rows > 0) {
        while ($row = $result->fetch_assoc()) {
            $results[] = $row;
        }
    } else {
        $message = "No results found for '$searchQuery'.";
    }

    // Close connection
    $conn->close();
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title>Search Results</title>
  <link rel="stylesheet" href="../css/header.css">
  <link rel="stylesheet" href="../css/search_handler.css">
  <link rel="stylesheet" href="../css/footer.css">
  <link rel="icon" href="../img/icon.png">
</head>
<body>
  <!-- Header -->
  <div class="header">
    <a href="#" class="logo"><img src="../img/2024.png">ESports<br>Festival</a>
    <div class="header-right">
      <a href="../index.html">Home</a>
      <a href="../index2.html">About</a>
      <a href="../index3.html">Register</a>
      <a href="../index4.html">ContactUs</a>
    </div><br>
    <div class="searchbox">
      <form action="search_handler.php" method="GET">
        <button type="submit">Search</button>
        <input type="text" name="search" placeholder="Enter a keyword..." required>
      </form>
    </div>
  </div>

  <!-- Content -->
  <div class="content">
    <div class="content-box">
      <h2>Search Results</h2>
      <?php if (!empty($results)): ?>
        <table>
          <thead>
            <tr>
              <th>Username</th>
              <th>Role</th>
            </tr>
          </thead>
          <tbody>
            <?php foreach ($results as $row): ?>
              <tr>
                <td>
                  <a href="profile_handler.php?id=<?php echo urlencode($row['id']); ?>">
                   <?php echo htmlspecialchars($row['username']); ?>
                  </a>
                </td>
                <td><?php echo htmlspecialchars($row['role']); ?></td>
              </tr>
            <?php endforeach; ?>
          </tbody>
        </table>
      <?php else: ?>
        <p class="no-results"><?php echo $message ?? "No results found."; ?></p>
      <?php endif; ?>
    </div>
  </div>

  <!-- Footer -->
  <footer>
    <h4>© Copyright 2024 | ESports Festival</h4>
  </footer>
</body>
</html>

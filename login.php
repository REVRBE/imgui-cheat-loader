<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Handle form submission
    $username = $_POST['username'];
    $password = $_POST['password'];

    // Replace these values with your actual database credentials
    $servername = "your_host_name";
    $db_username = "your_database_username";
    $db_password = "your_database_password";
    $db_name = "your_database_name";

    $conn = new mysqli($servername, $db_username, $db_password, $db_name);

    if ($conn->connect_error) {
        die(json_encode(['success' => false, 'message' => 'Connection failed: ' . $conn->connect_error]));
    }

    $hashed_password = getPassword($username, $conn);

	if ($hashed_password && checkPassword($password, $hashed_password)) {
    $user_rank = getUserRank($username, $conn);
    echo json_encode(['success' => true, 'user_rank' => $user_rank]);
} else {
    echo json_encode(['success' => false, 'message' => 'Incorrect username or password']);
}
    $conn->close();
}

function getPassword($username, $conn) {
    if ($stmt2 = mysqli_prepare($conn, "SELECT data FROM xf_user_authenticate WHERE user_id = (SELECT user_id FROM xf_user WHERE username = ?)")) {
        mysqli_stmt_bind_param($stmt2, "s", $username);
        mysqli_stmt_execute($stmt2);
        mysqli_stmt_bind_result($stmt2, $password);
        mysqli_stmt_fetch($stmt2);
        mysqli_stmt_close($stmt2);

        if ($password !== null) {
            $password = substr($password, 22, -3);
            return $password;
        }
    }
}

function checkPassword($user_pass, $hashed_pass){
    if (password_verify($user_pass, $hashed_pass)) {
        return true;
    }
    return false;
}

function getUserRank($username, $conn) {
    if ($stmt = mysqli_prepare($conn, "SELECT xf_user_group.title, xf_user.vip_end_time FROM xf_user_group INNER JOIN xf_user ON FIND_IN_SET(xf_user_group.user_group_id, xf_user.secondary_group_ids) OR xf_user_group.user_group_id = xf_user.user_group_id WHERE xf_user.username = ? ORDER BY xf_user_group.display_style_priority DESC LIMIT 1")) {
        mysqli_stmt_bind_param($stmt, "s", $username);
        mysqli_stmt_execute($stmt);
        mysqli_stmt_bind_result($stmt, $user_rank, $vip_end_time);
        mysqli_stmt_fetch($stmt);
        mysqli_stmt_close($stmt);

        if ($user_rank !== null) {
            if ($user_rank === 'VIP') {
                return ['user_rank' => $user_rank, 'vip_end_time' => $vip_end_time];
            }
            return ['user_rank' => $user_rank];
        }
    }
    return null;
}
?>

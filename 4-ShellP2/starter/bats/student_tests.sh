#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "exit command" {
    run "./dsh" <<EOF                
exit
EOF

    [ "$status" -eq 0 ]
}

@test "Background execution" {
    run "./dsh" <<EOF                
sleep 1 &
EOF

    sleep 0.5
    ps aux | grep "sleep" > /dev/null

    [ "$status" -eq 0 ]
}

@test "Exit command with argument" {
    run "./dsh" <<EOF                
exit 0
EOF

    [ "$status" -eq 0 ]
}

{
    "targets": [
        {
            "target_name": "discordrpc",
            "sources": ["rpc.cc"],
            "include_dirs": [
                "include"
            ],
            "library_dirs": [
                "lib"
            ],
            "libraries": [
                "-ldiscord-rpc"
            ]
        }
    ]
}
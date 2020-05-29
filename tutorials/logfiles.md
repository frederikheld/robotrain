# How to read logfiles

Logfiles are used to store messages that would otherwise logged to the terminal. Each line represents one message. You can read logfiles in different ways.

## Display full file (for short files)

```sh
$ cat logfile.log
```

This will print the full current content of the file to the terminal. As the terminal can only show a limited number of lines, this might cut off longer files.

## Display full file (for long files)

The better alternative is `less`, which adds scrolling to the output:

```sh
$ less logfile.log
```

You can now scroll through the file with the <kbd>↑</kbd> and <kbd>↓</kbd> arrow keys. To close the file, press <kbd>q</kbd>.

## Display the 15 most recent logs

New logs are written to the end of the file and sometimes you're interested in the most recent logs. In this case you can use `tail`, which will show the last 15 lines of the file:

```sh
$ tail logfile.log
```

## Keep the logs running

The methods above will only show what's in the logfile at the moment you issue the command. While you're reading the output, new lines might be added to the file which you will not see unless you run the command again.

To debug ongoing communication, it is sometimes useful to see the log messages as they are written to the file. The `tail` command mentioned above has an option that does that:

```sh
$ tail -f logfile.log
```

Will print out logs from the file live as if they were directly written to the command line.
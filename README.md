# palm-message-extraction

If you, like me, are an obsessive data hoarder and want to pull messages from
your old Palm/Treo phone, this repository is for you.

Using the Palm backup and sync program (or an SD card) you should be able to
extract your `Messages Database.pdb`, which contains all of your text messages.

The `.pdb` format is akin to PalmOS's filesystem. The `prc/` directory in this
repository contains a program (`par`) that can break these `.pdb` files into
records, which you can think of like individual files. In the case of text
messages, each message is a record.

Example:
```bash
./prc/par x messages-database.pdb
```
Warning: this makes a LOT of files if you have a lot of texts. These records
are still hard to read though. That's where the `treomsg/` directory comes in.

The `.pdr` files the `par` utility creates can then be extracted using
`dumpmsg` inside of `treomsg/`.

Example:
```bash
./treomsg/dumpmsg -r some-file.pdr
```
This dumps the contents of the (interpreted) `.pdr` file to stdout.

The source code for `treomsg/` at least has been modified from its original
form to fix bugs. You can find the original code for both `treomsg/` and `prc/`
in `src-archives/`.

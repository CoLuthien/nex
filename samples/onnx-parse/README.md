# onnx-parse

Reads a quantized export through the onnx frontend and then asks whether what came back is a graph
anything could be lowered from.

```
./build/bin/sample-onnx-parse ~/workspace/lm-exporter/out/gemma4-quant/chunk_0/chunk_0.seq1.onnx
```

The model is the only argument. Its weight files and its encodings sit beside it and are found
there: every sibling `*.onnx.data` the model actually names -- a graph rewritten from another one
keeps pointing at the weights of the model it came from, so `chunk_0.seq1.onnx` reads
`chunk_0.onnx.data` -- and `<stem>.encodings`. Both can be given instead with `--data` and
`--encodings`; `--help` lists the rest.

The exit code is 0 when every check that decides it passed.

## What it is for

Parsing that does not throw is not the same as parsing that worked. An export of this size arrives
in three files -- the graph, its weights, and the encodings that say how those weights are
quantized -- and each of them can be read successfully while still being paired with the wrong
other two. A window taken at the wrong offset of a mapped weight file is bytes either way, and
encodings belonging to a different export name the same tensors.

So the sample loads the model and then goes back over it through the public frontend interface,
which is the same surface a backend is written against: a gap here is a gap whoever picks the
loader up next would meet.

| check | what it would catch |
| --- | --- |
| graph interface | inputs and outputs the graph lists but its value table does not hand out, or that arrive with no type or shape to allocate from |
| value table | a value reachable by index but not by name, or listed twice |
| operand resolution | an operand naming a value nothing in the graph describes, or a layer whose view of a value disagrees with the graph's |
| dependency order | a layer reading a value written after it -- the topological sort the loader does rather than trusts -- or two layers writing the same one |
| operator signatures | operators the schema registry has no signature for at the opset the model imports, and layers whose operand count does not fit the signature they were resolved to (advisory) |
| weights | a weight unreachable by name or by index, sized past the region behind it, or whose bytes cannot be read to the last element |
| quantization parameters | a scale that is missing, not f32, not positive, or counted differently from the channels it is meant to cover; a zero point sized differently from the scale; a bitwidth of zero |
| encodings against the weights | weights that are not a multiple of the scale their encodings describe, i.e. encodings paired with a different export (advisory) |

Two of the checks are advisory: they report on the model rather than on the loader and never decide
the exit code. A model is allowed to hold operators this frontend has no schema for, and an export
is allowed to leave a weight in float while still writing an encoding for it.

Weights are sampled rather than read whole -- 4096 elements each, spread across the tensor, since a
weight file mapped at the wrong offset looks perfectly ordinary for its first few elements. Pass
`--sample 0` to read every element of every weight instead.

## Reading the output

```
-- checks --
ok   weights                                      1271 examined
       259 tensors, 1848 MiB, 590058 elements read back
note encodings against the weights they describe    222 examined
       156 of 222 weights hold exactly the values their encoding can represent, 66 do not
     - _to_copy_25: 852 of 1536 sampled values are a multiple of its 16-bit scale ...
```

`ok` and `note` both mean the run is sound; `FAIL` is the only one that is not, and it is what the
exit code follows.

On the gemma4-quant chunk above, the 66 weights that are not on their grid are the norm weights the
export left in float while still writing 16-bit encodings for them. That is the model saying
something about itself, which is why it is a note and not a failure -- but the same line is what
would show up if the encodings of another export were handed in, so it is worth reading rather than
skipping.

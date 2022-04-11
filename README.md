# BenthicClassifier
Classification of benthic point clouds

## Generate labels

First, we need to generate Hackel features for all our point cloud data using, and clean lines with NaN:
```
cat xyz_text_file | soundings_generate_features radius >> outputfile.Hackel
sed -i '/nan/d' "${WORKDIR}/WithFeatures/outputfile.Hackel"

```

Example radius values:
- For MBES: 10
- For LiDAR: 100


## Match labels with ground truthing

Use the generate-training-data.py script to generate labeled training data:

```
python3 src/generate-training-data.py radius path-to-dfo-labels.csv directory-containing-dot-Hackel-files > training-data.txt
```

Example radius:
- For MBES: 3
- For LiDAR: 10

## Train model

Using the traning data text file, train the model:

```
python3 src/train-model.py training-data.txt
```

This will generate a trained.model binary file that can be loaded with pickle in Python scripts or used in the following section

## Using the model

1) Generate features for the new files with soundings_generate_features:


2) Use the apply.py script to apply the model to a new text file:

```
python3 src/apply-model.py trained.model new_xyz_file.Hackel
```


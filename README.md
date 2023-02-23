# BenthicClassifier
Classification of benthic point clouds

## Step 1: Generating labels

We need to generate Hackel features for point cloud data using, and clean lines with NaN:
```
cat xyz_text_file | ./soundings_generate_features radius >> outputfile.Hackel
```

Example radius values:
- For MBES: 10
- For LiDAR: 100

or process a MBES batch
```
bash script/hackel_batch.bash directory outputDirectory radius
```

## Step 2: Matching labels with ground truthing

Use the generate-training-data.py script to generate labeled training data:

```
python3 src/generate-training-data.py radius path-to-dfo-labels.csv directory-containing-dot-Hackel-files > training-data.txt
```

Example radius:
- For MBES: 3
- For LiDAR: 10

## Step 3: Training the model

Using the traning data text file, train the model:

```
python3 src/train-model.py training-data.txt
```

This will generate a trained.model binary file that can be loaded with pickle in Python scripts or used in the following section

## Step 4: Using the model

1) Generate features for the new files with soundings_generate_features:


2) Use the apply.py script to apply the model to a new text file:

```
python3 src/apply-model.py trained.model new_xyz_file.Hackel
```

# BGX vs GMM

### Step 1: Train both model
```
python3 src/apply-model.py trained.model new_xyz_file.Hackel
python3 src/gmm-best-fit.py new_xyz_file.Hackel max_nb_class
```

### Step 2: Apply model
```
python3 src/apply-both-model.py model-file gmm-model-file hackelDataFile > outputClassification.csv
```
or in batch
```
bash script/classify-mbes-data.bash hackelDirectory
```

### Step 3: Generate a comparative table and 3D bar plot
```
python3 src/compare-model.py classifiedData
```
or in batch
```
bash script/gen_table_figure.bash classificationsDirectory
```

### Step 4: Generate raster
```
bash script/genraster.bash classificationsDirectory resolutionX resolutionY epsgCode
```
